import * as React from 'react';
import { useTags } from './useTags';
import { AppShell } from './AppShell';
import { TagTable } from './TagTable';
import { TagInfo, TagInfoService } from './TagInfoService';

export const App: React.FC = props => {
  const {} = props;

  const tags = useTags();

  const handleAlarmRequest = React.useCallback(
    async (tag: TagInfo) => {
      alert(`Tag ${tag.name} (${tag.macAddress}) was requested to be alarmed.`);
      await TagInfoService.requestAlarm(tag.macAddress);
    },
    []
  );

  return (
    <AppShell>
      <TagTable
        tags={tags}
        onRequestAlarm={handleAlarmRequest}
      />
    </AppShell>
  );
}
