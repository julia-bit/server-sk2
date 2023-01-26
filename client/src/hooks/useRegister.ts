import React from "react";
import { createUserWithEmailAndPassword } from "firebase/auth";
import { useMutation } from "@tanstack/react-query";
import { auth } from "../lib/firebase";
import { FirebaseError } from "firebase/app";
import useSocket from "./useSocket";

function useRegister() {
  const login = async ({
    email,
    password,
    confirm,
    socket,
  }: {
    socket: WebSocket | null;
    email?: string;
    password?: string;
    confirm?: string;
  }) => {
    if (!email || email.trim() === "") {
      throw new FirebaseError("auth/invalid-email", "Email must not be empty");
    }
    if (!password || password?.trim() === "") {
      throw new FirebaseError(
        "auth/invalid-password",
        "Password must not be empty"
      );
    }

    if (password !== confirm) {
      throw new FirebaseError(
        "auth/invalid-password",
        "Password must not be empty"
      );
    }

    if (!socket) {
      throw new FirebaseError("no socket provided", "no socket provided");
    }

    try {
      const cred = await createUserWithEmailAndPassword(auth, email, password);

      console.log(cred.user.uid + " " + cred.user.email);

      socket.send(
        JSON.stringify({
          operation: "/newUser",
          user_id: cred.user.uid,
          username: cred.user.email,
        })
      );
    } catch (error: any) {
      console.log(error);
      throw new Error(error.message);
    }
  };

  const { socket } = useSocket();

  return useMutation({
    mutationFn: async (v: {
      email?: string;
      password?: string;
      confirm?: string;
    }) => await login({ ...v, socket }),
  });
}

export default useRegister;
