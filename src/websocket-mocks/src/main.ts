import { ArgumentParser } from "argparse";
import util = require("util");
import { Client } from "./client";
import { logger } from "./logger";
import { SocketIoClient } from "./socketio-client";
import { WebSocketClient } from "./websocket-client";

const parser = new ArgumentParser();
parser.addArgument(["-s", "--server"]);
parser.addArgument(["-c", "--client"]);
parser.addArgument(["-t", "--token"]);

const args = parser.parseArgs();

logger.debug(`Using server: ${args.server}`);
logger.debug(`Using client: ${args.client}`);

let client = new Client(args.server, 1000);
switch (args.client) {
  case "socketio":
    logger.debug("Using socketio.");
    client = new SocketIoClient(args.server, 1000, args.token);
    break;
  case "websocket":
    logger.debug("Using websocket.");
    client = new WebSocketClient(args.server, 1000);
    break;
  default:
    logger.warn(`Socket type ${args.client}  is not supported.`);
    logger.info("Supported types are 'socketio' or 'websocket'");
    process.exit(-1);
}
client.onMessage((data: any) => {
  logger.debug(`Message received: ${util.inspect(data)}`);
});
client.start();
