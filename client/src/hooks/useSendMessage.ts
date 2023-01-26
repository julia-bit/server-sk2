import React from "react";
import { useMutation } from "@tanstack/react-query";
import { User as FirebaseUser } from "firebase/auth";
import useUser from "@/hooks/useUser";
import useSocket from "@/hooks/useSocket";

const sendMessage = ({
  user,
  socket,
  recieverId,
  text,
}: {
  user: FirebaseUser | null;
  socket: WebSocket | null;
  recieverId?: string;
  text?: string;
}) => {
  if (!user || !socket || !text || !recieverId) {
    console.log("provide all");
    return;
  }
  const message = {
    from: user.uid,
    to: recieverId,
    message: text,
    timestamp: new Date().toISOString(),
  };

  socket.send(JSON.stringify({ operation: "/message", ...message }));
};

function useSendMessage() {
  const { user } = useUser();
  const { socket } = useSocket();

  return useMutation({
    mutationFn: async ({
      text,
      recieverId,
    }: {
      text?: string;
      recieverId?: string;
    }) => {
      sendMessage({ text, user, recieverId, socket });
    },
    onMutate: () => {},
    onError: () => {},
  });
}

export default useSendMessage;
