import axios from "axios";

export interface TagInfo {
  name: string;
  macAddress: string;
  isConnected: boolean;
  lastSeen: Date;
}

const getAll = async (): Promise<TagInfo[]> => {
  const { data } = await axios.get<TagInfo[]>("http://localhost:8000/tags");
  
  return data.map(data => ({
    ...data,
    lastSeen: new Date(data.lastSeen)
  }));
}

const requestAlarm = async (tagMacAddres: string): Promise<void> => {
  await axios.put(`http://localhost:8000/tags/${tagMacAddres}`);
}

export const TagInfoService = {
  getAll,
  requestAlarm
}