#!/bin/bash
function write_key_value() {
    local key=$1
    local new_value=$2
    local file=$3

    # 替换 `=` 到 `#` 之间的值
    sed -i "s/^\($key=\)[^#]*/\1$new_value /" "$file"
}

function read_key_value() {
    local key=$1
    local file=$2
    value=$(grep "^$key=" "$file" | sed 's/^[^=]*=\([^#]*\).*/\1/' | xargs)
    echo "$value"
}
