#!/usr/bin/env python3
# -*- coding:utf-8 -*-
# 输入必须为utf8
import re


#
class CommandMatcher:
    def __init__(self, data, cmd):
        # 初始化数据列表和命令列表
        self.data = data
        self.cmd = cmd
        # 将数据列表转换为字典
        self.key_cmd_map = {item["key"] for item in data}

    def find_keys_in_text(self, text):
        found_keys = []
        # 使用正则表达式来确保即使关键词是其他词的一部分也能被匹配
        for key in self.key_cmd_map:
            for match in re.finditer(re.escape(key), text):
                found_keys.append((match.start(), key))

        # 根据关键词出现的位置进行排序
        found_keys.sort()

        # 生成结果列表
        result = [key for _, key in found_keys]

        return result

    # def match_command(self, input_string):
    #     keys = self.find_keys_in_text(input_string)  # 去除无用信息
    #     print("keys=", keys, "input_string=", input_string)

    #     for command in self.cmd:
    #         for match_string in command["match"]:
    #             if "".join(keys) == match_string:
    #                 return command["cmd"]
    #     return None

    def match_command(self, input_string):
        keys = self.find_keys_in_text(input_string)
        print("keys=", keys, "input_string=", input_string)

        for command in self.cmd:
            for match_keys in command["match"]:
                if set(match_keys).issubset(keys):
                    return command["cmd"]
        return None
if __name__ == "__main__":
    # 初始化数据
    data = [
        {"key": "保存"},
        {"key": "当前"},
        {"key": "位置"},
        {"key": "清除"},
        {"key": "去"},
        {"key": "前"},
        {"key": "往"},
        {"key": "下"},
        {"key": "一"},
        {"key": "个"},
        {"key": "点"},
        {"key": "上"},
        {"key": "清除"},
        {"key": "看"},
        {"key": "转"},
        {"key": "我"},
        {"key": "朝向"},
    ]
    cmd = [
        {"cmd": 0x01, "match": [["保存", "当前位置"], ["保存", "位置"], ["保存", "当前"]]},
        {"cmd": 0x02, "match": [["清除", "当前位置"], ["清除", "位置"], ["清除", "所有"]]},
        {"cmd": 0x03, "match": [["去", "下", "一个", "点"], ["往", "下", "一个", "点"]]},
        {"cmd": 0x04, "match": [["去", "上", "一个", "点"], ["往", "上", "一个", "点"]]},
        {"cmd": 0x05, "match": [["往", "前", "一个", "点"], ["去", "前", "一个", "点"]]},
        {"cmd": 0x06, "match": [["清除"]]},
        {"cmd": 0x07, "match": [["看", "我"], ["看看", "我"],["朝向", "我"]]},
    ]   

    # 测试数据
    test = [
        "保存当前点位",
        "清除所有点位",
        "看看我",
        "朝向我",
    ]

    matcher = CommandMatcher(data, cmd)

    # 测试find_keys_in_text方法
    for text in test:
        keys = matcher.find_keys_in_text(text)
        print(f"Text: '{text}' -> Keys: {keys}")

    # 测试match_command方法
    input_strings =test
    for input_string in input_strings:
        command = matcher.match_command(input_string)
        print(
            f"Input: '{input_string}' -> Command: {hex(command) if command else 'None'}"
        )
