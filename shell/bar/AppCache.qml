import Quickshell
import Quickshell.Io
import QtQuick

Item {
    id: root

    property var apps: []
    property string jsonBuffer: ""

    function reload() {
        if (scanner.running)
            return;
        root.jsonBuffer = "";
        scanner.running = true;
    }

    Component.onCompleted: {
        root.reload();
    }

    Timer {
        interval: 180000
        running: true
        repeat: true
        onTriggered: {
            root.reload();
        }
    }

    Process {
        id: scanner
        command: ["python3", "-c", "import os, glob, json\n" + "dirs = ['/usr/share/applications', '/var/lib/flatpak/exports/share/applications', os.path.expanduser('~/.local/share/flatpak/exports/share/applications'), os.path.expanduser('~/.local/share/applications')]\n" + "apps = {}\n" + "for d in dirs:\n" + "    if not os.path.exists(d): continue\n" + "    for f in glob.glob(os.path.join(d, '**/*.desktop'), recursive=True):\n" + "        try:\n" + "            name, icon, exec_cmd, nodisp = '', '', '', False\n" + "            with open(f, 'r', encoding='utf-8', errors='ignore') as fp:\n" + "                for line in fp:\n" + "                    line = line.strip()\n" + "                    if line.startswith('Name=') and not name:\n" + "                        name = line[5:]\n" + "                    elif line.startswith('Icon=') and not icon:\n" + "                        icon = line[5:]\n" + "                    elif line.startswith('Exec=') and not exec_cmd:\n" + "                        exec_cmd = line[5:]\n" + "                    elif line == 'NoDisplay=true':\n" + "                        nodisp = True; break\n" + "            if not nodisp and name and (exec_cmd or icon):\n" + "                app_id = os.path.splitext(os.path.basename(f))[0]\n" + "                if name not in apps:\n" + "                    apps[name] = {'id': app_id, 'name': name, 'icon': icon or app_id, 'exec': exec_cmd.split('%')[0].strip()}\n" + "        except Exception:\n" + "            pass\n" + "out = list(apps.values())\n" + "out.sort(key=lambda x: x['name'].lower())\n" + "print(json.dumps(out))"]
        running: false

        stdout: SplitParser {
            onRead: data => {
                root.jsonBuffer += data;
            }
        }

        onExited: (code, status) => {
            if (code === 0 && root.jsonBuffer.trim() !== "") {
                try {
                    root.apps = JSON.parse(root.jsonBuffer);
                } catch (e) {}
            }
            root.jsonBuffer = "";
        }
    }
}
