import React, { useRef } from "react";
import useRegister from "../../../hooks/useRegister";
import { useRouter } from "next/router";
import Link from "next/link";
import { FirebaseError } from "firebase/app";
import { formatFireabseAuthError } from "../../../helpers/firebaseLoginErrors";
import { GetServerSidePropsContext } from "next";
import { loggedInRedirect } from "@/helpers/loggedInRedirect";
import useSocket from "@/hooks/useSocket";

function Register() {
  const email = useRef<HTMLInputElement | null>(null);
  const password = useRef<HTMLInputElement | null>(null);
  const confirm = useRef<HTMLInputElement | null>(null);

  const { socket } = useSocket();

  const router = useRouter();

  const { mutateAsync, error } = useRegister();

  const register = async (e: React.FormEvent) => {
    e.preventDefault();
    try {
      await mutateAsync({
        email: email.current?.value,
        password: password.current?.value,
        confirm: confirm.current?.value,
      });
      // router.push("/");
    } catch (error) {
      console.error(error);
    }
  };
  return (
    <div className="flex h-screen max-h-screen w-screen items-center bg-slate-800 text-white">
      <form
        onSubmit={register}
        className="md: mx-auto flex w-full max-w-sm flex-col rounded-xl border p-2 md:border-slate-50 md:p-6 "
      >
        <h1 className="text-3xl font-semibold">Register</h1>
        {error instanceof FirebaseError && (
          <div className="mt-6 rounded-lg bg-red-500 p-2 text-center text-red-50 ">
            {formatFireabseAuthError(error)}
          </div>
        )}
        <label className="mt-6" htmlFor="email">
          E-mail
        </label>
        <input className="input mt-2" name="email" ref={email} />
        <label className="mt-6" htmlFor="password">
          Password
        </label>
        <input
          type={"password"}
          className="input mt-2"
          name="password"
          ref={password}
        />
        <label className="mt-6" htmlFor="password">
          Confirm Password
        </label>
        <input
          type={"password"}
          className="input mt-2"
          name="password"
          ref={confirm}
        />
        <div className="mt-8 flex items-center justify-between">
          <button type="submit" className="button">
            Sign up
          </button>
          <div className="text-xs ">
            Already have an account?{" "}
            <Link className="text-slate-300 underline" href={"/auth/login"}>
              Log In
            </Link>
          </div>
        </div>
      </form>
    </div>
  );
}

export function getServerSideProps(context: GetServerSidePropsContext) {
  return loggedInRedirect(context);
}

export default Register;
