// xf_tts_node.cpp
#include <cstdio>
#include <cstring>
#include <string>
#include <unistd.h>

#include "xf_voice/qtts.h"
#include "xf_voice/msp_cmn.h"
#include "xf_voice/msp_errors.h"

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "xf_voice_msgs/srv/xf.hpp"      // ✅ 与 srv/Xf.srv 对应

using std::string;
using namespace std::chrono_literals;

/* ---------- WAV 头 ---------- */
#pragma pack(push,1)
struct WavePcmHdr {
  char  riff[4]  {'R','I','F','F'};
  int   size_8   {0};
  char  wave[4]  {'W','A','V','E'};
  char  fmt[4]   {'f','m','t',' '};
  int   fmt_size {16};
  short format_tag      {1};
  short channels        {1};
  int   samples_per_sec {16000};
  int   avg_bytes_sec   {32000};
  short block_align     {2};
  short bits_per_sample {16};
  char  data[4]  {'d','a','t','a'};
  int   data_size{0};
};
#pragma pack(pop)
static WavePcmHdr kDefaultHdr;

/* ---------- 合成核心 ---------- */
static int text_to_speech(const char *text,
                          const char *out_path,
                          const char *params)
{
  if (!text || !out_path) return -1;
  unsigned int len = 0;
  int synth_stat = 0;

  FILE *fp = fopen(out_path, "wb");
  if (!fp) return -1;

  int ret = MSP_SUCCESS;
  const char *sid = QTTSSessionBegin(params, &ret);
  if (ret != MSP_SUCCESS) { fclose(fp); return ret; }

  ret = QTTSTextPut(sid, text, (unsigned)strlen(text), nullptr);
  if (ret != MSP_SUCCESS) goto fail;

  fwrite(&kDefaultHdr, sizeof(kDefaultHdr), 1, fp);

  kDefaultHdr.data_size = 0;
  synth_stat = MSP_TTS_FLAG_STILL_HAVE_DATA;

  while (synth_stat == MSP_TTS_FLAG_STILL_HAVE_DATA) {
    const void *data = QTTSAudioGet(sid, &len, &synth_stat, &ret);
    if (ret != MSP_SUCCESS) goto fail;
    if (data && len > 0) {
      fwrite(data, len, 1, fp);
      kDefaultHdr.data_size += len;
    }
    usleep(150000);
  }

  kDefaultHdr.size_8 = kDefaultHdr.data_size + sizeof(kDefaultHdr) - 8;
  fseek(fp, 4,  SEEK_SET); fwrite(&kDefaultHdr.size_8, 4, 1, fp);
  fseek(fp, 40, SEEK_SET); fwrite(&kDefaultHdr.data_size, 4, 1, fp);

fail:
  fclose(fp);
  QTTSSessionEnd(sid, "Normal");
  return ret;
}

/* ---------- ROS2 节点 ---------- */
class XfTtsNode : public rclcpp::Node {
public:
  XfTtsNode() : Node("xf_tts_node")
  {
    /* 1. 读取参数（带默认） */
    this->get_parameter_or("tts_appid",  tts_appid_,  string("9a7a7ebd"));
    this->get_parameter_or("out_name",   out_name_,   string("/tmp/tts.wav"));
    this->get_parameter_or("voice_name", voice_name_, string("xiaoyan"));
    this->get_parameter_or("voice_text_enc", text_enc_,   string("utf8"));
    this->get_parameter_or("voice_sample_rate", sample_rate_, string("16000"));
    this->get_parameter_or("voice_speed",  speed_,  string("50"));
    this->get_parameter_or("voice_volume", volume_, string("50"));
    this->get_parameter_or("voice_pitch",  pitch_,  string("50"));
    this->get_parameter_or("voice_rdn",    rdn_,    string("2"));
    this->get_parameter_or("play_after_tts", play_after_tts_, true);

    /* 2. 登录讯飞 */
    string login = "appid = " + tts_appid_ + ", work_dir = .";
    if (MSPLogin(nullptr, nullptr, login.c_str()) != MSP_SUCCESS) {
      RCLCPP_FATAL(get_logger(), "MSPLogin failed");
      rclcpp::shutdown();
      return;
    }

    /* 3. 订阅话题 */
    sub_ = create_subscription<std_msgs::msg::String>(
      "tts_topic", rclcpp::SensorDataQoS(),
      std::bind(&XfTtsNode::voiceCallback, this, std::placeholders::_1));

    /* 4. 创建服务 */
    srv_ = create_service<xf_voice_msgs::srv::Xf>(
      "tts_service",
      std::bind(&XfTtsNode::serviceCallback,
                this, std::placeholders::_1, std::placeholders::_2));
  }

  ~XfTtsNode() override { MSPLogout(); }

private:
  /* ---------- 回调 ---------- */
  void voiceCallback(const std_msgs::msg::String::SharedPtr msg)
  {
    run_tts(msg->data);
  }

  void serviceCallback(
    const std::shared_ptr<xf_voice_msgs::srv::Xf::Request>  req,
    std::shared_ptr<xf_voice_msgs::srv::Xf::Response>       res)
  {
    int ret = run_tts(req->text);
    res->success = (ret == MSP_SUCCESS);
    res->message = res->success ? "合成成功" :
                   ("合成失败，错误码：" + std::to_string(ret));
  }

  /* ---------- 工具 ---------- */
  int run_tts(const string &text)
  {
    string params = "voice_name = " + voice_name_ +
                    ", text_encoding = " + text_enc_ +
                    ", sample_rate = " + sample_rate_ +
                    ", speed = " + speed_ +
                    ", volume = " + volume_ +
                    ", pitch = " + pitch_ +
                    ", rdn = " + rdn_;

    int ret = text_to_speech(text.c_str(), out_name_.c_str(), params.c_str());
    if (ret == MSP_SUCCESS && play_after_tts_) {
     // std::string cmd = "play " + out_name_ + " >/dev/null 2>&1 &"; 
      std::string cmd = "play " + out_name_ + " >/dev/null 2>&1 "; //非阻塞
      system(cmd.c_str());
    }
    return ret;
  }

  /* ---------- 成员 ---------- */
  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr sub_;
  rclcpp::Service<xf_voice_msgs::srv::Xf>::SharedPtr srv_;

  /* 参数 */
  string tts_appid_, out_name_, voice_name_,
         text_enc_, sample_rate_, speed_, volume_, pitch_, rdn_;
  bool   play_after_tts_;
};

/* ---------- main ---------- */
int main(int argc, char **argv)
{
  setlocale(LC_CTYPE, "zh_CN.utf8");
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<XfTtsNode>());
  rclcpp::shutdown();
  return 0;
}
