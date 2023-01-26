import React, { useState } from "react";
import useUser from "@/hooks/useUser";
import { Message } from "@/components/ChatMessage";

export const WS_URL = "ws://localhost:1101";
export const REST_URL = "http://localhost:1100";

type SocketContextType = {
  socket: WebSocket | null;
  messages: Message[];
};

export const SocketContext = React.createContext<SocketContextType>({
  socket: null,
  messages: [],
});

function SocketProvider({ children }: React.PropsWithChildren) {
  const { user, loading, error } = useUser();
  const [socket, setSocket] = useState<WebSocket | null>(null);

  React.useEffect(() => {
    if (socket) {
      return () => {
        socket.close();
      };
    }

    if (!user) {
      return;
    }

    const ws = new WebSocket(WS_URL);

    ws.onopen = function () {
      console.log("connected");
      ws.send(
        JSON.stringify({
          operation: "/newUser",
          user_id: user?.uid ?? "",
          username: user?.email,
        })
      );
    };

    ws.onmessage = function (evt) {
      const msg = JSON.parse(evt.data);
      console.log(msg);
    };

    ws.onclose = function () {
      console.log("disconnected");
    };

    setSocket(ws);

    return () => {
      // ws.close();
    };
  }, [user]);

  return (
    <SocketContext.Provider value={{ socket, messages: [] }}>
      {children}
    </SocketContext.Provider>
  );
}

export default SocketProvider;
