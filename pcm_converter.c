#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

// 定义输入和输出参数
#define INPUT_SAMPLE_RATE 8000      // 输入采样率
#define OUTPUT_SAMPLE_RATE 48000    // 输出采样率
#define INPUT_CHANNELS 1            // 输入通道数（单声道）
#define OUTPUT_CHANNELS 2           // 输出通道数（双声道）

// 上采样函数（线性插值法）
void resample(int16_t *input, int input_length, int16_t *output, int output_length) {
    float step = (float)input_length / output_length;
    for (int i = 0; i < output_length; i++) {
        float index = i * step;
        int idx1 = (int)index;
        int idx2 = idx1 + 1;

        // 边界处理
        if (idx2 >= input_length) idx2 = input_length - 1;
        if (idx1 >= input_length) idx1 = input_length - 1;

        // 线性插值
        float fraction = index - idx1;
        output[i] = (int16_t)((1.0f - fraction) * input[idx1] + fraction * input[idx2]);
    }
}

int main() {
    // 打开输入文件
    FILE *input_file = fopen("/usr/samples/origin.g711a", "rb");
    if (!input_file) {
        perror("无法打开输入文件");
        return EXIT_FAILURE;
    }

    // 获取文件大小并读取数据
    fseek(input_file, 0, SEEK_END);
    long file_size = ftell(input_file);
    fseek(input_file, 0, SEEK_SET);

    int input_length = file_size / sizeof(int16_t);
    int16_t *input_data = (int16_t *)malloc(file_size);
    fread(input_data, sizeof(int16_t), input_length, input_file);
    fclose(input_file);

    // 计算输出长度
    int output_length = (int)((long long)input_length * OUTPUT_SAMPLE_RATE / INPUT_SAMPLE_RATE);

    // 分配上采样后的缓冲区
    int16_t *resampled_data = (int16_t *)malloc(output_length * sizeof(int16_t));
    resample(input_data, input_length, resampled_data, output_length);

    // 单声道转双声道
    int stereo_length = output_length * OUTPUT_CHANNELS;
    int16_t *stereo_data = (int16_t *)malloc(stereo_length * sizeof(int16_t));
    for (int i = 0; i < output_length; i++) {
        stereo_data[i * 2] = resampled_data[i];       // 左声道
        stereo_data[i * 2 + 1] = resampled_data[i];   // 右声道
    }

    // 写入输出文件
    FILE *output_file = fopen("/usr/samples/output_48k_stereo.pcm", "wb");
    if (!output_file) {
        perror("无法打开输出文件");
        free(input_data);
        free(resampled_data);
        free(stereo_data);
        return EXIT_FAILURE;
    }

    fwrite(stereo_data, sizeof(int16_t), stereo_length, output_file);
    fclose(output_file);

    // 释放内存
    free(input_data);
    free(resampled_data);
    free(stereo_data);

    printf("转换完成！\n");
    return EXIT_SUCCESS;
}