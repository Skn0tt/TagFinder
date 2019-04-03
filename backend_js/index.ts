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
    try {
      await axios.post(`http://${tag.ip}:8000/alarm`);
    } catch (e) {}
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

app.put("/tags/:macAddress", async (req, res) => {
  const { macAddress } = req.params;

  await sendAlarm(macAddress);

  res.status(200);
  res.end();
});

const removeFFFF = (i: string): string => i.split("::ffff:")[1];

app.post("/tags/:macAddress/:name", (req, res) => {
  const ip = req.connection.remoteAddress;
  const { macAddress, name } = req.params;

  addTag({
    isConnected: true,
    lastSeen: Date.now(),
    macAddress,
    name,
    ip: removeFFFF(ip)
  });

  res.status(200);
  res.end();
});

async function tagIsAvailable(tag: TagInfo): Promise<boolean> {
  try {
    const response = await axios.get(`http://${tag.ip}:8000/status`, { timeout: 1000 });
    return response.status === 200;
  } catch (e) {
    return false;
  }
}

async function pingTags() {
  for (let macAddress of Object.keys(tags)) {
    const t = tags[macAddress];
    const isAvailable = await tagIsAvailable(t);
    t.isConnected = isAvailable;
    if (isAvailable) {
      t.lastSeen = Date.now();
    }
  }
}

setInterval(pingTags, 1000 * 5);


app.listen(8000);
console.log("Listening on 8000.");
