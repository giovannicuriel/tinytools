import { createLogger, format, transports } from "winston";

const logger = createLogger({
  format: format.combine(format.splat(), format.simple()),
  level: "debug",
  transports: [
    new transports.Console(),
  ],
});

export { logger };
