import ChatersProvider from "@/context/ChatersContext";
import UserProvider from "@/context/UserContext";
import "@/styles/globals.css";
import type { AppProps } from "next/app";
import { QueryClientProvider, QueryClient } from "@tanstack/react-query";
import SocketProvider from "@/context/SocketContext";

const client = new QueryClient();

export default function App({ Component, pageProps }: AppProps) {
  return (
    <QueryClientProvider client={client}>
      <UserProvider>
        <ChatersProvider>
          <SocketProvider>
            <Component {...pageProps} />
          </SocketProvider>
        </ChatersProvider>
      </UserProvider>
    </QueryClientProvider>
  );
}
