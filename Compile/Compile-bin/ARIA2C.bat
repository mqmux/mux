@echo off
printf 0x07 "%date:~5,5% %time:~0,-3% ["
printf 0x03 http://aria2c.com/
echos 0x07 ] (JSON-RPC Path) ws://127.0.0.1:6800/jsonrpc
down.exe --conf-path="%~dp0Sourse Code\aria2.conf"
