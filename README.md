# Steps
- Compile code:
```bash
g++ -o /usr/sbin/mydaemon main.cpp -lsystemd
```

- Create a service file
Create the file `/etc/systemd/system/mydaemon.service` and add the following:
```bash
[Unit]
Description=My Modern C++ Daemon
After=network.target

[Service]
ExecStart=/usr/sbin/mydaemon
Type=forking
Restart=on-failure
RestartSec=5s
StandardOutput=journal
StandardError=journal
User=atracy

[Install]
WantedBy=multi-user.target
```
- Start the Daemon
```bash
sudo systemctl daemon-reload
sudo systemctl start mydaemon
```

If you want it to start on reboot
```bash
sudo systemctl enable mydaemon
```

- Check status and logs
```bash
sudo systemctl status mydaemon
sudo cat /var/log/mydaemon.log
```


- Checking stdcout ad stderr logs, etc

NOTE: The changes that allowed for this were to update the daemon code:
```bash
# Removing a custome file logger
# replacing calls the the logger with
std::cout << "Daemon is running..." << std::endl;
```

and to update the configuration in `etc/systemd/system/mydaemon.service`.
```bash
# Adding these to the [Service] section
StandardOutput=journal
StandardError=journal
```

```bash
sudo journalctl -u mydaemon.service    # Shows all logs
sudo journalctl -u mydaemon.service -f # Realtime log tailing
sudo journalctl -u mydaemon.service -b # logs since last boot
sudo journalctl -u mydaemon.service --since "1 hour ago" # More specific time filtering
```
