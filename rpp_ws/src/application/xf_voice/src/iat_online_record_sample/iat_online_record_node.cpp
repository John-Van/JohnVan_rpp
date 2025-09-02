// xf_voice_listener.cpp  ——  ROS 2 版
#include <chrono>
#include <cstring>
#include <memory>
#include <thread>

#include "xf_voice/qisr.h"
#include "xf_voice/msp_cmn.h"
#include "xf_voice/msp_errors.h"
#include "xf_voice/speech_recognizer.h"

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/int8.hpp"

using namespace std::chrono_literals;

/* ----------------------------  全局运行标志  ---------------------------- */
static bool run_listening  = true;   // mic 采集是否进行中
static bool result_ready   = false;  // 已得到识别结果
static char *g_result      = nullptr;
static size_t g_buf_size   = 4096;

/* -------------------------  iat 结果回调 ------------------------- */
static void show_result(const char *str, bool is_over)
{
  RCLCPP_INFO(rclcpp::get_logger("xf_voice"), "Result: %s%s",
              str, is_over ? "\n" : "");
}

static void on_result(const char *result, char is_last)
{
  if (!result) return;

  size_t left = g_buf_size - 1 - strlen(g_result);
  size_t need = strlen(result);
  if (left < need) {
    g_result = static_cast<char *>(realloc(g_result, g_buf_size + 4096));
    g_buf_size += 4096;
  }
  strncat(g_result, result, need);

  if (is_last) {
    show_result(g_result, true);
    result_ready  = true;
    run_listening = false;
  }
}

static void on_speech_begin()
{
  if (g_result) free(g_result);
  g_buf_size = 4096;
  g_result   = static_cast<char *>(calloc(g_buf_size, 1));
  RCLCPP_INFO(rclcpp::get_logger("xf_voice"), "Start listening ...");
}

static void on_speech_end(int reason)
{
  if (reason == END_REASON_VAD_DETECT) {
    RCLCPP_INFO(rclcpp::get_logger("xf_voice"), "Speech end (VAD)");
  } else {
    RCLCPP_ERROR(rclcpp::get_logger("xf_voice"),
                 "Recognizer error: %d", reason);
  }
  run_listening = false;
}

/* --------------------------  ROS2 Node -------------------------- */
class XfVoiceListener : public rclcpp::Node
{
public:
  XfVoiceListener()
  : Node("xf_voice_listener")
  {
    /* ----------- 参数声明 & 读取 ----------- */
    this->declare_parameter<std::string>("iat_appid", "9a7a7ebd");
    this->declare_parameter<std::string>("package_path", "/home/");
    this->declare_parameter<int>("maxTime", 15);

    iat_appid_     = this->get_parameter("iat_appid").as_string();
    package_path_  = this->get_parameter("package_path").as_string();
    max_time_sec_  = this->get_parameter("maxTime").as_int();

    /* ----------- Publisher / Subscriber ----------- */
    voice_pub_ = this->create_publisher<std_msgs::msg::String>("xf/voice", 10);
    awake_sub_ = this->create_subscription<std_msgs::msg::Int8>(
      "/awake_flag", 10,
      std::bind(&XfVoiceListener::awakeCallback, this, std::placeholders::_1));

    /* ----------- 登录讯飞平台 ----------- */
    std::string login = "appid=" + iat_appid_ + ",work_dir=.";
    int ret = MSPLogin(nullptr, nullptr, login.c_str());
    if (ret != MSP_SUCCESS) {
      RCLCPP_FATAL(this->get_logger(),
                   "MSPLogin failed, error=%d. Exiting.", ret);
      rclcpp::shutdown();
      return;
    }
    RCLCPP_INFO(this->get_logger(), "MSPLogin success.");

    /* ----------- 启动主循环定时器 ----------- */
    timer_ = this->create_wall_timer(
      50ms, std::bind(&XfVoiceListener::spinOnce, this));
  }

  ~XfVoiceListener() override
  {
    MSPLogout();
    if (g_result) free(g_result);
  }

private:
  /* ----------------  唤醒回调 ---------------- */
  void awakeCallback(const std_msgs::msg::Int8::SharedPtr)
  {
    if (listening_) return;            // 已在录音中
    listening_ = true;
    result_ready = false;
    run_listening = true;

    RCLCPP_INFO(this->get_logger(), "Awake flag received, start IAT.");
    std::thread(&XfVoiceListener::runIat, this).detach();
  }

  /* ----------------  IAT 线程  ---------------- */
  void runIat()
  {
    const char *session_param =
      "sub=iat,domain=iat,language=zh_cn,accent=mandarin,"
      "sample_rate=16000,result_type=plain,result_encoding=utf8";

    struct speech_rec_notifier notifier { on_result,on_speech_begin,on_speech_end };
    struct speech_rec recognizer;
    int ret = sr_init(&recognizer, session_param, SR_MIC, &notifier);
    if (ret) {
      RCLCPP_ERROR(this->get_logger(), "sr_init failed: %d", ret);
      listening_ = false;
      return;
    }
    if (sr_start_listening(&recognizer)) {
      RCLCPP_ERROR(this->get_logger(), "sr_start_listening failed");
      sr_uninit(&recognizer);
      listening_ = false;
      return;
    }

    /* -------- 超时控制 -------- */
    for (int i = 0; i < max_time_sec_ && run_listening; ++i) {
      std::this_thread::sleep_for(1s);
    }
    sr_stop_listening(&recognizer);
    sr_uninit(&recognizer);
    listening_ = false;
  }

  /* ----------------  主循环 ---------------- */
  void spinOnce()
  {
    if (result_ready) {
      auto msg = std_msgs::msg::String();
      msg.data = g_result;
      voice_pub_->publish(msg);
      RCLCPP_INFO(this->get_logger(), "Published result.");
      result_ready = false;
  
      if (g_result) {
        free(g_result);
        g_result = nullptr;
      }
      g_buf_size = 4096;
    }
  }
  
  /* ----------------  成员变量 ---------------- */
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr voice_pub_;
  rclcpp::Subscription<std_msgs::msg::Int8>::SharedPtr awake_sub_;
  rclcpp::TimerBase::SharedPtr timer_;

  std::string iat_appid_;
  std::string package_path_;
  int max_time_sec_{15};
  std::atomic<bool> listening_{false};
};

/* ----------------------  main ---------------------- */
int main(int argc, char **argv)
{
  setlocale(LC_CTYPE, "zh_CN.utf8");      // 终端中文
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<XfVoiceListener>());
  rclcpp::shutdown();
  return 0;
}
