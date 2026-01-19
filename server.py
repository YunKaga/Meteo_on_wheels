import socket
from http.server import HTTPServer, BaseHTTPRequestHandler
import threading
import time
import urllib.parse

# === НАСТРОЙТЕ ЭТО ===
ESP_IP = "10.42.0.104"  # ← ЗАМЕНИТЕ на IP вашего ESP8266
ESP_PORT = 8080

latest_data = "Ожидание..."
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
            time.sleep(3)

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
            html = f'''
            <html><head><meta charset="utf-8"><title>Meteo Wheels</title>
            <style>button {{ width: 80px; height: 80px; font-size: 20px; margin: 5px; }}</style>
            </head><body>
              <h2>Данные с датчиков:</h2>
              <pre id="data">{latest_data}</pre>
              <p>Обновлено: <span id="time">--</span></p>

              <div style="margin: 20px;">
                <button onclick="send('CMD:F')">↑ W</button><br>
                <button onclick="send('CMD:L')">← A</button>
                <button onclick="send('CMD:S')">↓ S</button>
                <button onclick="send('CMD:R')">→ D</button>
              </div>
              <button onclick="send('SAVE_NOW')" style="font-size:18px;">💾 Сохранить сейчас</button>

              <script>
                function send(cmd) {{
                  fetch('/cmd?c=' + encodeURIComponent(cmd));
                }}
                function update() {{
                  fetch('/data').then(r => r.text()).then(t => {{
                    document.getElementById('data').innerText = t;
                    document.getElementById('time').innerText = new Date().toLocaleTimeString();
                  }});
                }}
                setInterval(update, 1000);
                update();
              </script>
            </body></html>
            '''
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
