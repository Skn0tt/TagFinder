# TagFinder

*TagFinder* is a small hardware-project that features a "Find My Phone"-like tagging system for your personal belongings.
It consists of small tags which can be remotely invoked to start an alarm, enabling you to recover your lost items.

The tags register themselves at a central repository server that publishes a web frontend for the user.

The project was realized during the HPI-MINT-Camp 2019.

## Getting Started

Use Platform.io to install the client software on a microcontroller (e.g. ESP32 or Arduino).
Make sure to adjust the IPs to your specific network topology.
Start the backend server using `ts-node index.ts`, start the frontend using `npm start`.

## Contributors

- [Tido  Specht](https://github.com/VanModers)
- Stefan Brand
- Maximilian Koch
- Leo
- [Simon Knott](https://github.com/skn0tt)