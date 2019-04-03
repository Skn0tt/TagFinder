import * as React from "react";
import MUIDataTable from "mui-datatables";
import * as _ from "lodash";
import { TagInfo } from "./TagInfoService";

interface TagTableProps {
  tags: TagInfo[];
  onRequestAlarm: (tag: TagInfo) => void;
}

export const TagTable: React.FC<TagTableProps> = props => {
  const { tags, onRequestAlarm } = props;

  const tagByMacAddress = React.useMemo(
    () => _.keyBy(
      tags,
      (tag: TagInfo) => tag.macAddress
    ),
    [tags]
  );

  const handleRowClick = React.useCallback(
    rowData => {
      const macAddress = rowData[1];
      const tag = tagByMacAddress[macAddress];
      if (!!tag) {
        onRequestAlarm(tag);
      }
    },
    [onRequestAlarm, tagByMacAddress]
  );

  return (
    <MUIDataTable
      title={"TagFinder"}
      data={tags}
      columns={[
        {
          name: "name",
          label: "Name"
        },
        {
          name: "macAddress",
          label: "MAC Address"
        },

        {
          name: "lastSeen",
          label: "Last Seen",
          options: {
            customBodyRender: (date: Date) => date.toLocaleDateString()
          }
        },
        {
          name: "isConnected",
          label: "Status",
          options: {
            customBodyRender: (isConnected: boolean) => isConnected ? "Connected": "Not Connected"
          }
        },
      ]}
      options={{
        onRowClick: handleRowClick,
        selectableRows: false,
      }}
    />
  )
}