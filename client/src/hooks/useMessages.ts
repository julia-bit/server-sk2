import React from "react";
import { useQuery } from "@tanstack/react-query";
import { REST_URL } from "@/context/SocketContext";
import useUser from "@/hooks/useUser";
import { Message } from "@/components/ChatMessage";
import useSocket from "./useSocket";

const fetchMessages = async (recieverId?: string, userId?: string) => {
  if (!userId || !recieverId) {
    return [];
  }

  try {
    const res = await fetch(REST_URL + "/messages", {
      headers: {
        "Content-type": "application/json",
      },
      method: "POST",
      body: JSON.stringify({ sender_id: userId, receiver_id: recieverId }),
    });
    const data: Message[] = await res.json();

    const result = data.sort((msgA, msgB) => {
      const [dateA, dateB] = [
        new Date(msgA.timestamp).getTime(),
        new Date(msgB.timestamp).getTime(),
      ];
      return dateB - dateA;
    });

    return result;
  } catch (err) {
    throw new Error("unexpected error when getting messages");
  }
  return [];
};

function useMessages(receiverId?: string) {
  const { user } = useUser();
  const { socket } = useSocket();

  return useQuery({
    queryFn: async () => await fetchMessages(receiverId, user?.uid),
    queryKey: ["messages", receiverId, user?.uid],
    refetchInterval: 500,
  });
}

export default useMessages;
