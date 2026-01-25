# keymap

Put [kanata](https://github.com/jtroo/kanata) + [kanata-tray](https://github.com/rszyma/kanata-tray) in the repo root, then run `kanata-tray`.

## Linux systemd Service

On Linux, you can use a systemd service to start kanata automatically at boot.

Create `/etc/systemd/system/kanata.service`:

```ini
[Unit]
Description=Kanata Service
Requires=local-fs.target
After=local-fs.target

[Service]
ExecStartPre=/sbin/modprobe uinput
ExecStart=/path/to/kanata -c /path/to/kanata.kbd --no-wait --quiet --nodelay
Restart=no

[Install]
WantedBy=sysinit.target
```

Then start and enable the service:

```bash
sudo systemctl start kanata.service
sudo systemctl enable kanata.service
```
