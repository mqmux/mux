from fastapi import FastAPI, UploadFile, File, HTTPException, Depends, Header, Body
from fastapi.security import APIKeyHeader
from fastapi.staticfiles import StaticFiles
from pydantic import BaseModel
import uuid
import os
import uvicorn

app = FastAPI()
UPLOAD_DIR = "uploads"
os.makedirs(UPLOAD_DIR, exist_ok=True)

# 只存储一个 Token（初始示例）
CURRENT_TOKEN = ""  # 默认 Token，可修改

# 定义 Token 验证方式（从 Header 获取）
API_TOKEN_HEADER = APIKeyHeader(name="X-Token")

def verify_token(token: str = Depends(API_TOKEN_HEADER)):
    """验证 Token 是否有效"""
    if token != CURRENT_TOKEN:
        raise HTTPException(status_code=403, detail="Invalid or missing Token")
    return token

app.mount("/static", StaticFiles(directory=UPLOAD_DIR), name="static")

# Pydantic 模型，用于 Token 修改请求
class TokenUpdateRequest(BaseModel):
    new_token: str  # 新 Token

@app.post("/upload")
async def upload_file(
    file: UploadFile = File(...),
    token: str = Depends(verify_token),  # 依赖 Token 验证
):
    """上传文件（需携带有效 Token）"""
    original_ext = os.path.splitext(file.filename)[1]
    unique_name = f"{uuid.uuid4()}{original_ext}"
    file_path = os.path.join(UPLOAD_DIR, unique_name)

    with open(file_path, "wb") as f:
        while True:
            chunk = await file.read(8192)
            if not chunk:
                break
            f.write(chunk)

    return {
        "url": f"/static/{unique_name}",
        "message": "File uploaded successfully"
    }

@app.post("/update_token")
async def update_token(
    request: TokenUpdateRequest,
    token: str = Depends(verify_token),  # 只有当前有效 Token 才能修改 Token
):
    if not request.new_token:
        raise HTTPException(status_code=400, detail="Token cannot be empty")
    """
    修改 Token（只能有一个 Token，不能新增）
    """
    global CURRENT_TOKEN
    CURRENT_TOKEN = request.new_token  # 更新 Token
    
    return {
        "message": "Token updated successfully",
        "new_token": CURRENT_TOKEN  # 返回新 Token（仅用于调试）
    }

if __name__ == "__main__":
    uvicorn.run("main:app", host="0.0.0.0", port=12025, reload=True)