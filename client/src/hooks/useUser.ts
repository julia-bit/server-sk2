import { UserContext } from "./../context/UserContext";
import { useContext } from "react";

function useUser() {
  return useContext(UserContext);
}

export default useUser;
