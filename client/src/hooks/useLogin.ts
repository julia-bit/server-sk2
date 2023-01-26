import React from "react";
import { signInWithEmailAndPassword } from "firebase/auth";
import { useMutation } from "@tanstack/react-query";
import { auth } from "../lib/firebase";
import { FirebaseError } from "firebase/app";

function useLogin() {
  const login = async ({
    email,
    password,
  }: {
    email?: string;
    password?: string;
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
    await signInWithEmailAndPassword(auth, email, password);
  };

  return useMutation({ mutationFn: login });
}

export default useLogin;
