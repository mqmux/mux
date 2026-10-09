#include <stdio.h>
#include <atlstr.h>
#include <windows.h>
#include <Mmsystem.h>

#pragma comment(lib,"Winmm.lib")

char lpTemp[256];

DWORD FCC(LPSTR lpStr)
{
    DWORD Number = lpStr[0] + lpStr[1] * 0x100 + lpStr[2] * 0x10000 + lpStr[3] * 0x1000000;
    return Number;
}

int main(int argc, char* argv[])
{
    if (argc == 1) {
        printf("Usage: %s [采样率8000] [位深度8/16/32] [录制时间5] [文件名record.wav]\n", argv[0]);
        return 0;
    }
    int SecTime = 8000;
    int recordTime = 5;
    int deeprecord = 8;
    char filename[1024];
    strcpy(filename, "record.wav");
    switch (argc)
    {
    case 5:
        strcpy(filename, argv[4]);
    case 4:
        recordTime = atoi(argv[3]);
    case 3:
        deeprecord = atoi(argv[2]);
    case 2:
        SecTime = atoi(argv[1]);
        break;
    default:
        printf("Parameter quantity：%d 参数过多\n",argc-1);
        return argc;
    }
    DWORD datasize = SecTime * recordTime;
    // 如果采样率是8000（每秒采样8000次）
    // 采样位数是8（每个采样8位），通道数是1（单声道）
    // 录音时间是10秒，那么需要的音频数据缓冲区大小就是8000 * 10 * 8 / 8 * 1 = 80000字节

    // 设置录音采样参数
    // 定义WAVEFORMATEX结构体变量，用于设置音频格式
    WAVEFORMATEX waveformat;
    // 设置音频格式为PCM
    waveformat.wFormatTag = WAVE_FORMAT_PCM;
    // 设置音频通道数为1，即单声道
    waveformat.nChannels = 1;
    // 设置采样率为8000，即每秒采样8000次
    waveformat.nSamplesPerSec = SecTime;
    // 设置块对齐，即每个采样需要的字节数
    waveformat.nBlockAlign = 1;
    // 设置每个采样的位数，这里是8位
    waveformat.wBitsPerSample = 8;
    // 设置额外信息的大小，这里没有额外信息，所以设置为0
    waveformat.cbSize = 0;
    // 设置平均字节速率，等于通道数*采样率*每个采样的位数/8
    waveformat.nAvgBytesPerSec = waveformat.nChannels * waveformat.nSamplesPerSec * waveformat.wBitsPerSample / 8;

    printf("WAVEFORMATEX size = %lu\n", sizeof(WAVEFORMATEX)); fflush(stdout);
    printf("音频数据缓冲区：%ld\n", datasize); fflush(stdout);
    HWAVEIN m_hWaveIn;
    if (!waveInGetNumDevs())
    {
        printf("没有可以使用的 WaveIn 通道\n"); fflush(stdout);
        return 0;
    }

    // 打开录音设备
    int res = waveInOpen(&m_hWaveIn, WAVE_MAPPER, &waveformat, (DWORD)NULL, 0L, CALLBACK_WINDOW);
    if (res != MMSYSERR_NOERROR)
    {
        printf("打开 waveIn 通道失败，Error_Code = 0x%x\n", res); fflush(stdout);
        return 0;
    }

    // 定义WAVEHDR结构体变量，用于描述音频数据缓冲区
    WAVEHDR m_pWaveHdr;
    // 为音频数据缓冲区分配内存，并获取内存的指针
    m_pWaveHdr.lpData = (char*)GlobalLock(GlobalAlloc(GMEM_MOVEABLE | GMEM_SHARE, datasize));
    // 将音频数据缓冲区的内存清零
    memset(m_pWaveHdr.lpData, 0, datasize);
    // 设置音频数据缓冲区的长度
    m_pWaveHdr.dwBufferLength = datasize;
    // 设置已经录制的音频数据的长度，初始值为0
    m_pWaveHdr.dwBytesRecorded = 0;
    // 用户自定义数据，这里没有使用，所以设置为0
    m_pWaveHdr.dwUser = 0;
    // 音频数据缓冲区的标志，这里没有特殊设置，所以设置为0
    m_pWaveHdr.dwFlags = 0;
    // 循环播放的次数，这里没有使用，所以设置为0
    m_pWaveHdr.dwLoops = 0;
    printf("WAVEHDR size = %lu\n", sizeof(WAVEHDR)); fflush(stdout);

    // 准备内存块录音
    int resPrepare = waveInPrepareHeader(m_hWaveIn, &m_pWaveHdr, sizeof(WAVEHDR));
    if (resPrepare != MMSYSERR_NOERROR)
    {
        printf("不能开辟录音头文件，Error_Code = 0x%03X\n", resPrepare); fflush(stdout);
        return 0;
    }

    resPrepare = waveInAddBuffer(m_hWaveIn, &m_pWaveHdr, sizeof(WAVEHDR));
    if (resPrepare != MMSYSERR_NOERROR)
    {
        printf("不能开辟录音用缓冲，Error_Code = 0x%03X\n", resPrepare); fflush(stdout);
        return 0;
    }

    if (!waveInStart(m_hWaveIn))
    {
        printf("开始录音\n"); fflush(stdout);
    }
    else
    {
        printf("开始录音失败\n"); fflush(stdout);
        return 0;
    }

    // 通过命令行参数控制录制时间
    for (int i = 0; i < recordTime; i++) {
        printf("."); fflush(stdout);
        Sleep(1000);
    }
    printf("\n"); fflush(stdout);

    if (!waveInStop(m_hWaveIn))
    {
        printf("暂停录音\n"); fflush(stdout);
    }
    else
    {
        printf("暂停录音失败\n"); fflush(stdout);
        return 0;
    }


    MMTIME mmt;
    mmt.wType = TIME_BYTES;
    printf("sizeof(MMTIME) = %d, sizeof(UINT) = %d\n", sizeof(MMTIME), sizeof(UINT)); fflush(stdout);

    if (!waveInGetPosition(m_hWaveIn, &mmt, sizeof(MMTIME)))
    {
        printf("取得现在音频位置\n"); fflush(stdout);
    }
    else
    {
        printf("不能取得音频长度\n"); fflush(stdout);
        return 0;
    }

    if (mmt.wType != TIME_BYTES)
    {
        printf("指定的 TIME_BYTES 格式音频长度不支持\n"); fflush(stdout);
        return 0;
    }

    if (!waveInStop(m_hWaveIn))
    {
        printf("停止录音\n"); fflush(stdout);
    }
    else
    {
        printf("停止录音失败\n"); fflush(stdout);
    }

    if (waveInReset(m_hWaveIn))
    {
        printf("重置内存区失败\n"); fflush(stdout);
        return 0;
    }

    m_pWaveHdr.dwBytesRecorded = mmt.u.cb;
    DWORD NumToWrite = 0;
    DWORD dwNumber = 0;
    HANDLE FileHandle = CreateFile(CString(filename), GENERIC_WRITE,
        FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    // 清空音频数据缓冲区
    // memset(m_pWaveHdr.lpData, 0, datasize);
    // 写入RIFF标识
    dwNumber = FCC(LPSTR("RIFF"));
    WriteFile(FileHandle, &dwNumber, 4, &NumToWrite, NULL);
    // 写入文件总长度，等于音频数据长度 + 头部信息长度
    dwNumber = m_pWaveHdr.dwBytesRecorded + 18 + 20;
    WriteFile(FileHandle, &dwNumber, 4, &NumToWrite, NULL);
    // 写入WAVE标识
    dwNumber = FCC(LPSTR("WAVE"));
    WriteFile(FileHandle, &dwNumber, 4, &NumToWrite, NULL);
    // 写入fmt标识
    dwNumber = FCC(LPSTR("fmt "));
    WriteFile(FileHandle, &dwNumber, 4, &NumToWrite, NULL);
    // 写入WAVEFORMATEX结构体长度，固定为18
    dwNumber = 18L;
    WriteFile(FileHandle, &dwNumber, 4, &NumToWrite, NULL);
    // 写入WAVEFORMATEX结构体，包含了音频数据的格式信息
    WriteFile(FileHandle, &waveformat, sizeof(WAVEFORMATEX), &NumToWrite, NULL);
    // 写入data标识
    dwNumber = FCC(LPSTR("data"));
    WriteFile(FileHandle, &dwNumber, 4, &NumToWrite, NULL);
    // 写入音频数据长度
    dwNumber = m_pWaveHdr.dwBytesRecorded;
    WriteFile(FileHandle, &dwNumber, 4, &NumToWrite, NULL);
    // 写入音频数据
    WriteFile(FileHandle, m_pWaveHdr.lpData, m_pWaveHdr.dwBytesRecorded, &NumToWrite, NULL);
    // 设置文件结束标记
    SetEndOfFile(FileHandle);
    // 关闭文件句柄
    CloseHandle(FileHandle);
    // 将文件句柄设置为无效值
    FileHandle = INVALID_HANDLE_VALUE;
    printf("已生成 %s 文件\n", filename); fflush(stdout);

    if (waveInUnprepareHeader(m_hWaveIn, &m_pWaveHdr, sizeof(WAVEHDR)))
    {
        printf("Un_Prepare Header 失败\n"); fflush(stdout);
    }
    else
    {
        printf("Un_Prepare Header 成功\n"); fflush(stdout);
        return 0;
    }

    if (GlobalFree(GlobalHandle(m_pWaveHdr.lpData)))
    {
        printf("Global Free 失败\n"); fflush(stdout);

    }
    else
    {
        printf("Global Free 成功\n"); fflush(stdout);
    }

    if (res == MMSYSERR_NOERROR) // 关闭录音设备
    {
        if (waveInClose(m_hWaveIn) == MMSYSERR_NOERROR)
        {
            printf("正常关闭录音设备\n"); fflush(stdout);
        }
        else
        {
            printf("非正常关闭录音设备\n"); fflush(stdout);
            return 0;
        }
    }
}