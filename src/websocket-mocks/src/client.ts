import { logger } from "./logger";

/**
 * Class for communications using WebSockets
 */
class Client {

  /** Server to connect */
  protected server: string;

  /** Interval between reconnections */
  protected autoReconnectInterval: number;

  /** Callback invoked when a message is received */
  protected onMessageCb?: (data: any) => void;

  /**
   * Constructor
   * @param server The server that this websocket client will connect to
   * @param autoReconnectInterval Interval between reconnections
   */
  constructor(server: string, autoReconnectInterval: number) {
    this.server = server;
    this.autoReconnectInterval = autoReconnectInterval;
    this.onMessageCb = undefined;
  }

  /**
   * Set the callback for received messages
   * @param onMessage The callback to be invoked when a message is received
   */
  public onMessage(onMessage: (data: any) => void) {
    this.onMessageCb = onMessage;
  }

  /**
   * Start the websocket handling
   */
  public start() {
    logger.debug("Could not start client - abstract method invoked");
  }

  /**
   * Reconnect the websocket, if any error occured previously.
   */
  public reconnect() {
    logger.debug("Could not reconnect client - abstract method invoked");
  }
}

export { Client };
