import * as React from "react";
import { AppBar, Toolbar, Typography } from "@material-ui/core";

export const AppShell: React.FC = props => {
  const { children } = props;

  return (
    <div className="App">
      {/**
      <AppBar position="static">
        <Toolbar>
          <Typography variant="h6" color="inherit">
            TagFinder
          </Typography>
        </Toolbar>
      </AppBar>
       */}
      

      {children}
    </div>
  );
}