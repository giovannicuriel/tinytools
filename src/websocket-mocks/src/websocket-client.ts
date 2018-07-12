import WebSocket = require("ws");
import { Client } from "./client";
import { logger } from "./logger";

/**
 * Class for communications using WebSockets
 */
class WebSocketClient extends Client {

  /** WebSocket handler */
  private websocket?: WebSocket;

  /**
   * Constructor
   * @param server The server that this websocket client will connect to
   * @param autoReconnectInterval Interval between reconnections
   */
  constructor(server: string, autoReconnectInterval: number) {
    super(server, autoReconnectInterval);
    this.websocket = undefined;
  }

  /**
   * Start the websocket handling
   */
  public start() {
    logger.debug(`Creating connection to server: ${this.server}`);
    this.websocket = new WebSocket(this.server);
    logger.debug(`WebSocket was created.`);

    this.websocket.on("open", () => {
      logger.debug("open ws connection");
    });

    this.websocket.on("message", (data: WebSocket.Data) => {
      if (this.onMessageCb) {
        this.onMessageCb(data);
      } else {
        logger.debug("No message callback was set.");
      }
    });

    this.websocket.on("close", (code: number) => {
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

    this.websocket.on("error", (event: any) => {
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
    if (this.websocket) {
      this.websocket.removeAllListeners();
      setTimeout(() => {
        logger.debug("WebSocketClient: reconnecting...");
        this.start();
      }, this.autoReconnectInterval);
    }
  }
}

export { WebSocketClient };
