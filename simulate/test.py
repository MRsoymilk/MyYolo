import base64
import requests

with open("test.jpg","rb") as f:
    img = base64.b64encode(f.read()).decode()

data = {
    "name":"test",
    "type":"classify",
    "data":img
}

r = requests.post("http://127.0.0.1:18000/inference", json=data)

print(r.json())
