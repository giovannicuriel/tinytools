import * as sio from "socket.io-client";
import { Client } from "./client";
import { logger } from "./logger";

class SocketIoClient extends Client {

    private socketIo: SocketIOClient.Socket;

  /**
   * Constructor
   * @param server The server that this websocket client will connect to
   * @param autoReconnectInterval Interval between reconnections
   */
  constructor(server: string, autoReconnectInterval: number, token: string) {
    super(server, autoReconnectInterval);
    logger.debug(`Creating connection to server: ${this.server}, using token ${token}`);
    const query: SocketIOClient.ConnectOpts = {
        query: {
            token,
        },
        transports: ["polling"],
    };
    this.socketIo = sio(this.server, query);
    logger.debug(`SocketIO was created.`);
  }

  /**
   * Start the websocket handling
   */
  public start() {

    this.socketIo.on("all", (data: any) => {
      logger.debug("Received message");
      if (this.onMessageCb) {
        this.onMessageCb(data);
      } else {
        logger.debug("No message callback was set.");
      }
    });

    this.socketIo.on("close", (code: number) => {
      switch (code) {
        case 1000: // CLOSE_NORMAL
          logger.debug("WebSocket: closed");
          break;
        default:
          // Abnormal closure
          this.reconnect();
          break;
      }
    });

    this.socketIo.on("error", (event: any) => {
      switch (event.code) {
        case "ECONNREFUSED":
          this.reconnect();
          break;
        default:
          logger.debug("Error ws connection: " + event);
          break;
      }
    });
  }

  /**
   * Reconnect the websocket, if any error occured previously.
   */
  public reconnect() {
    logger.debug(`WebSocketClient: retry in ${this.autoReconnectInterval}ms`);
    if (this.socketIo) {
      this.socketIo.removeAllListeners();
      setTimeout(() => {
        logger.debug("WebSocketClient: reconnecting...");
        this.start();
      }, this.autoReconnectInterval);
    }
  }
}

export { SocketIoClient };
