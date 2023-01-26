import { ChatersContext } from "@/context/ChatersContext";
import { useContext } from "react";
import { REST_URL } from "@/context/SocketContext";
import { User } from "@/context/ChatersContext";
import { useQuery } from "@tanstack/react-query";
import useUser from "@/hooks/useUser";

const fetchChatters = async (userId?: string) => {
  if (!userId) {
    return [];
  }

  try {
    const res = await fetch(REST_URL + "/users", {
      headers: {
        "Content-type": "application/json",
      },
      method: "POST",
    });
    const data: User[] = await res.json();

    return data.filter((user) => user.user_id != userId) ?? [];
  } catch (err) {
    //TODO: handle errors
  }
};

function useChaters() {
  const { user } = useUser();

  return useQuery({
    queryFn: async () => await fetchChatters(user?.uid),
    queryKey: ["users", user?.uid],
  });
}

export default useChaters;
