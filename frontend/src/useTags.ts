import * as React from "react";
import { useInterval } from "./useInterval";
import { TagInfoService, TagInfo } from "./TagInfoService";

export const useTags = (): TagInfo[] => {
  const [ tags, setTags ] = React.useState<TagInfo[]>([]);

  const onUpdate = React.useCallback(
    async () => {
      const response = await TagInfoService.getAll();
      setTags(response);
    },
    [setTags]
  );

  useInterval(onUpdate, 1000);

  return tags;
}