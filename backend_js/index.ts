import * as express from "express";
import axios from "axios";
import * as morgan from "morgan";
import * as cors from "cors";

interface TagInfo {
  name: string;
  macAddress: string;
  isConnected: boolean;
  lastSeen: number;
  ip: string;
}

const tags: Record<string, TagInfo> = {};

function addTag(tagInfo: TagInfo) {
  tags[tagInfo.macAddress] = tagInfo;
}

function getTag(macAddress: string): TagInfo | undefined {
  return tags[macAddress];
}

async function sendAlarm(macAddress: string) {
  const tag = getTag(macAddress);
  if (!!tag) {
    await axios.post(`http://${tag.ip}/`);
  }
}

const app = express();

app.use(morgan("combined"));

app.use(cors());

app.get("/tags", (req, res) => {
  const allTags = Object.values(tags);
  
  res.status(200);
  res.json(allTags);
  res.end();
});

app.post("/tags", (req, res) => {
  const ip = req.ip;
  const { macAddress, name } = req.body;

  addTag({
    isConnected: true,
    lastSeen: Date.now(),
    macAddress,
    name,
    ip
  });
});

app.post("/tags/:macAddress/alarm", async (req, res) => {
  const { macAddress } = req.params;

  await sendAlarm(macAddress);

  res.status(200);
  res.end();
})

app.listen(8000);
console.log("Listening on 8000.")