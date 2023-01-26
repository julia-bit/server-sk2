import React from "react";
import useUser from "@/hooks/useUser";

export type Message = {
  sender_id: string;
  receiver_id: string;
  message: string;
  timestamp: string;
};

function ChatMessage({ sender_id, receiver_id, message, timestamp }: Message) {
  const { user } = useUser();

  const align =
    user?.uid === receiver_id ? "bg-slate-600" : "bg-gray-500 self-end";

  return (
    <div className={`my-2 w-fit max-w-sm rounded-lg p-2 ${align}`}>
      {message}
    </div>
  );
}

export default ChatMessage;
