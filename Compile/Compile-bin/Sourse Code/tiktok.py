# https://github.com/davidteather/TikTok-Api?tab=readme-ov-file#installing

from TikTokApi import TikTokApi
import asyncio
import os
from datetime import datetime
import json

ms_token = os.environ.get("ms_token", None)  # 从环境变量中获取 ms_token

def printf(response):
    print(json.dumps(response.json(), indent=4, ensure_ascii=False))
    
async def get_video_info(video_url):
    async with TikTokApi() as api:
        # 创建 API 会话
        await api.create_sessions(ms_tokens=[ms_token], num_sessions=1, sleep_after=3)
        
        # 解析视频链接，获取视频对象（同步方式）
        video = api.video(url=video_url)
        count = 0
        async for comment in video.comments(count=30):
            print(comment)
            # print(comment.as_dict)
        
            # 追加写入到 JSON 文件
            with open('2.json', 'a', encoding='utf-8') as f:
                # 如果不是第一个评论，先写入逗号分隔符
                if f.tell() != 0:
                    f.write(',')
                json.dump(comment.as_dict, f, ensure_ascii=False, indent=4)
                f.write('\n')  # 写入换行符，使得每个评论占一行
            
            
        # 获取视频信息
        video_data = await video.info()
        
        # video_json = json.dumps(video_data, ensure_ascii=False, indent=4)
        # print(video_json)
        
       
        # 将视频信息按 JSON 格式输出到 1.json 文件
        with open('1.json', 'w', encoding='utf-8') as f:
            json.dump(video_data, f, ensure_ascii=False, indent=4)
            
        # 获取点赞数和播放量
        likes = video_data['stats']['diggCount']
        views = video_data['stats']['playCount']
        
        comments = video_data['stats']['commentCount']  # 评论数
        # favorites = video_data['stats']['favoriteCount']  # 收藏数
        shares = video_data['stats']['shareCount']  # 分享数
        description = video_data['desc']  # 正文
        author_name = video_data['author']['uniqueId']  # 作者账号名称
        create_time = video_data['createTime']  # 创建时间戳
        
        
        # 打印视频的点赞数和播放量
        print(f"Likes: {likes}")
        print(f"Views: {views}")
        
        print(f"Comments: {comments}")
        # print(f"Favorites: {favorites}")
        print(f"Shares: {shares}")
        print(f"Description: {description}")
        print(f"Author: {author_name}")
        
        # 将创建时间戳格式化为 YYYY-MM-DD
        # create_time_formatted = datetime.fromtimestamp(create_time).strftime('%Y-%m-%d')
        # print(f"Create Time: {create_time_formatted}")


if __name__ == "__main__":
    # 示例视频链接
    video_url = "https://www.tiktok.com/@xtoolofficial/video/7379058382981958945"
    
    # 运行异步函数
    asyncio.run(get_video_info(video_url))