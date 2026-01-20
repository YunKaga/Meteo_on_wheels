import socket
from http.server import HTTPServer, BaseHTTPRequestHandler
import threading
import time
import urllib.parse

# === НАСТРОЙТЕ ЭТО ===
ESP_IP = "192.168.137.183"  # ← ЗАМЕНИТЕ на IP вашего ESP8266
ESP_PORT = 8080

latest_data = ""
last_update = 0

def esp_listener():
    global latest_data, last_update
    while True:
        try:
            with socket.socket() as s:
                s.connect((ESP_IP, ESP_PORT))
                print("Подключено к ESP8266")
                buf = ""
                while True:
                    chunk = s.recv(128).decode('utf-8', errors='ignore')
                    if not chunk:
                        break
                    buf += chunk
                    while '\n' in buf:
                        line, buf = buf.split('\n', 1)
                        line = line.strip()
                        if line.startswith("DATA:"):
                            latest_data = line[5:]
                            last_update = time.time()
                        elif line:
                            print("ESP:", line)
        except Exception as e:
            print("Ошибка подключения к ESP:", e)
            time.sleep(1)

def send_to_esp(cmd):
    try:
        with socket.socket() as s:
            s.connect((ESP_IP, ESP_PORT))
            s.sendall((cmd + "\n").encode())
            print("Отправлено:", cmd)
    except Exception as e:
        print("Ошибка отправки:", e)

class WebHandler(BaseHTTPRequestHandler):
    def do_GET(self):
        if self.path == '/':
            self.send_response(200)
            self.send_header("Content-type", "text/html; charset=utf-8")
            self.end_headers()
            html = open("web.html", 'r', encoding="UTF-8").read()
            self.wfile.write(html.encode())
        elif self.path.startswith('/cmd'):
            cmd = urllib.parse.parse_qs(urllib.parse.urlparse(self.path).query).get('c', [''])[0]
            send_to_esp(cmd)
            self.send_response(200)
            self.end_headers()
        elif self.path == '/data':
            self.send_response(200)
            self.send_header("Content-type", "text/plain; charset=utf-8")
            self.end_headers()
            self.wfile.write(latest_data.encode())

if __name__ == '__main__':
    threading.Thread(target=esp_listener, daemon=True).start()
    print("Веб-интерфейс запущен на http://localhost:8000")
    HTTPServer(("", 8000), WebHandler).serve_forever()
