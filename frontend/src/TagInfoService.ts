import axios from "axios";

export interface TagInfo {
  name: string;
  macAddress: string;
  isConnected: boolean;
  lastSeen: Date;
}

const mockTags = [
  {
    name: "exampleTag",
    macAddress: "acbdefg",
    isConnected: true,
    lastSeen: new Date("2019-03-01T15:00:00+01:00")
  }
];


const getAll = async (): Promise<TagInfo[]> => {
  const { data } = await axios.get<TagInfo[]>("http://localhost:8000/tags");
  
  return data.map(data => ({
    ...data,
    lastSeen: new Date(data.lastSeen)
  }));
}

const requestAlarm = async (tagMacAddres: string): Promise<void> => {
  await axios.post("http://localhost:8000/alarm", null, { params: { id: tagMacAddres }});
}

export const TagInfoService = {
  getAll,
  requestAlarm
}