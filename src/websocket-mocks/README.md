# websocket-mocks

websocket-mocks contains a few clients and servers using websocket-like technologies

## How to use

Build:

```bash
npm install --only=dev
npm install
npm run build
npm start -- -s "http://localhost:8000/" -c socketio -t f6196ef4-ecc2...
```

The arguments are:

- -s: Server
- -c: which client should be used. Valid options are "socketio" and "websocket"
- -t: token. Valid for socketio clients.
