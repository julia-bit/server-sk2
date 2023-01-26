import React, { useEffect, useState } from "react";

export type User = {
  uid: string;
  email: string;
};

export const ChatersContext = React.createContext<{
  users: User[];
  loading: boolean;
}>({ users: [], loading: false });

function ChatersProvider({ children }: { children: React.ReactNode }) {
  const [loading, setLoading] = useState(false);

  return (
    <ChatersContext.Provider
      value={{
        users: [],
        loading,
      }}
    >
      {children}
    </ChatersContext.Provider>
  );
}

export default ChatersProvider;
