- Cài Node-red trên Local 
- Mở Powershell và ghi node-red
- Sau đó nó sẽ cho http://<localip>:<port>
- Truy cập vào bằng website và sau đó nhìn góc phải và tìm Import
- Sau đó paste dòng JSON này 
///
[
    {
        "id": "dc9c08a37bca77ca",
        "type": "tab",
        "label": "CCTV SYSTEM",
        "disabled": false,
        "info": ""
    },
    {
        "id": "a3f7f0c8c096b3f2",
        "type": "junction",
        "z": "dc9c08a37bca77ca",
        "x": 520,
        "y": 320,
        "wires": [
            [
                "03974bb661becb58"
            ]
        ]
    },
    {
        "id": "330868e10692b3cd",
        "type": "ui_template",
        "z": "dc9c08a37bca77ca",
        "group": "18e3cc337b3d1d4f",
        "name": "Stream Cam1",
        "order": 1,
        "width": 6,
        "height": 6,
        "format": "<div style='text-align:center'>\n<h3>Camera 1</h3>\n<img src='http://192.168.1.201:81/stream' style='width:100%; border-radius:10px;'>\n</div>",
        "storeOutMessages": false,
        "fwdInMessages": false,
        "resendOnRefresh": true,
        "templateScope": "local",
        "x": 180,
        "y": 60,
        "wires": [
            []
        ]
    },
    {
        "id": "35bece63680b370c",
        "type": "ui_template",
        "z": "dc9c08a37bca77ca",
        "group": "ad6792a1c7b2e8a2",
        "name": "Stream Cam2",
        "order": 1,
        "width": 6,
        "height": 6,
        "format": "<div style='text-align:center'>\n<h3>Camera 2</h3>\n<img src='http://192.168.1.202:81/stream' style='width:100%; border-radius:10px;'>\n</div>",
        "storeOutMessages": false,
        "fwdInMessages": false,
        "resendOnRefresh": true,
        "templateScope": "local",
        "x": 440,
        "y": 60,
        "wires": [
            []
        ]
    },
    {
        "id": "0c05ea1c268672af",
        "type": "http in",
        "z": "dc9c08a37bca77ca",
        "name": "Heartbeat API",
        "url": "/heartbeat",
        "method": "post",
        "upload": false,
        "x": 150,
        "y": 140,
        "wires": [
            [
                "8fa90e29db74f80d"
            ]
        ]
    },
    {
        "id": "8fa90e29db74f80d",
        "type": "json",
        "z": "dc9c08a37bca77ca",
        "x": 310,
        "y": 140,
        "wires": [
            []
        ]
    },
    {
        "id": "c36c8ba1a2518044",
        "type": "function",
        "z": "dc9c08a37bca77ca",
        "name": "Process Heartbeat",
        "func": "// msg.payload từ ESP gửi lên\nlet data = msg.payload;\n\n// Nếu ESP gửi dạng string thì parse\nif (typeof data === \"string\") {\n    try {\n        data = JSON.parse(data);\n    } catch(e) {\n        return null;\n    }\n}\n\n// Lấy dữ liệu\nlet status = data.status || \"UNKNOWN\";\nlet temp = data.temp || 0;\n\n// Output 1: Status text\nlet msg1 = { payload: status };\n\n// Output 2: Temperature gauge\nlet msg2 = { payload: Number(temp) };\n\n// Output 3: Alert text\nlet msg3 = { payload: (status === \"FIRE\") ? \"🚨 FIRE ALERT!\" : \"Normal\" };\n\nreturn [msg1, msg2, msg3];\n",
        "outputs": 4,
        "timeout": "",
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 510,
        "y": 140,
        "wires": [
            [
                "148e2fbc51c9c1b2"
            ],
            [
                "3f30163e9b40ebbb"
            ],
            [
                "01cbb7c725a40cda"
            ],
            [
                "a3f7f0c8c096b3f2"
            ]
        ]
    },
    {
        "id": "03974bb661becb58",
        "type": "http response",
        "z": "dc9c08a37bca77ca",
        "x": 770,
        "y": 440,
        "wires": []
    },
    {
        "id": "148e2fbc51c9c1b2",
        "type": "ui_text",
        "z": "dc9c08a37bca77ca",
        "group": "18e3cc337b3d1d4f",
        "order": 2,
        "width": "",
        "height": "",
        "name": "",
        "label": "Status:",
        "format": "{{msg.payload}}",
        "layout": "",
        "className": "",
        "style": false,
        "font": "",
        "fontSize": "",
        "color": "#000000",
        "x": 940,
        "y": 40,
        "wires": []
    },
    {
        "id": "3f30163e9b40ebbb",
        "type": "ui_gauge",
        "z": "dc9c08a37bca77ca",
        "name": "",
        "group": "18e3cc337b3d1d4f",
        "order": 3,
        "width": "",
        "height": "",
        "gtype": "gage",
        "title": "Temperature:",
        "label": "°C",
        "format": "",
        "min": 0,
        "max": "100",
        "colors": [
            "#00b500",
            "#e6e600",
            "#ca3838"
        ],
        "seg1": "",
        "seg2": "",
        "diff": false,
        "className": "",
        "x": 930,
        "y": 160,
        "wires": []
    },
    {
        "id": "01cbb7c725a40cda",
        "type": "ui_text",
        "z": "dc9c08a37bca77ca",
        "group": "18e3cc337b3d1d4f",
        "order": 4,
        "width": "",
        "height": "",
        "name": "",
        "label": "ALERT:",
        "format": "{{msg.payload}}",
        "layout": "",
        "className": "",
        "style": false,
        "font": "",
        "fontSize": "",
        "color": "#000000",
        "x": 940,
        "y": 300,
        "wires": []
    },
    {
        "id": "a0a4ba8bc80dd0ec",
        "type": "inject",
        "z": "dc9c08a37bca77ca",
        "name": "",
        "props": [
            {
                "p": "payload"
            }
        ],
        "repeat": "",
        "crontab": "",
        "once": false,
        "onceDelay": 0.1,
        "topic": "",
        "payload": "{\"status\":\"ONLINE\",\"temp\":28,\"alert\":\"NORMAL\"}",
        "payloadType": "json",
        "x": 130,
        "y": 360,
        "wires": [
            [
                "c36c8ba1a2518044"
            ]
        ]
    },
    {
        "id": "18e3cc337b3d1d4f",
        "type": "ui_group",
        "name": "CAMERA 1",
        "tab": "cc1a256716f492ea",
        "order": 1,
        "disp": true,
        "width": "6"
    },
    {
        "id": "ad6792a1c7b2e8a2",
        "type": "ui_group",
        "name": "CAMERA 2",
        "tab": "cc1a256716f492ea",
        "order": 2,
        "disp": true,
        "width": "6"
    },
    {
        "id": "cc1a256716f492ea",
        "type": "ui_tab",
        "name": "CCTV DASHBOARD",
        "icon": "dashboard",
        "order": 1
    },
    {
        "id": "6abacb7906281a08",
        "type": "global-config",
        "env": [],
        "modules": {
            "node-red-dashboard": "3.6.6"
        }
    }
]
///

- Sau đó Deploy 
- Rồi tạo tab mới ghi http://<localip>:<port>/ui

[
	Nhớ bấm góc phải và bấm Manage Palette: và tài thử viện dashboard và node base 64
]