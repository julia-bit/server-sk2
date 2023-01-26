import React from "react";
import { SocketContext } from "@/context/SocketContext";

function useSocket() {
  return React.useContext(SocketContext);
}

export default useSocket;
