import { useRouter } from "next/router";
import useLogin from "../../../hooks/useLogin";
import { useRef } from "react";
import Link from "next/link";
import { FirebaseError } from "firebase/app";
import { formatFireabseAuthError } from "@/helpers/firebaseLoginErrors";
import { GetServerSidePropsContext } from "next";
import { loggedInRedirect } from "@/helpers/loggedInRedirect";

function Login() {
  const { mutateAsync, error } = useLogin();
  const router = useRouter();

  const email = useRef<HTMLInputElement | null>(null);
  const password = useRef<HTMLInputElement | null>(null);

  const login = async (e: React.FormEvent) => {
    e.preventDefault();
    try {
      await mutateAsync({
        email: email.current?.value,
        password: password.current?.value,
      });
      router.push("/");
    } catch (error) {
      console.error(error);
    }
  };

  return (
    <div className="flex h-screen max-h-screen w-screen items-center bg-slate-800 text-white">
      <form
        onSubmit={login}
        className="md: mx-auto flex w-full max-w-sm flex-col rounded-xl border p-2 md:border-slate-50 md:p-6 "
      >
        <h1 className="text-3xl font-semibold ">Login</h1>
        {error instanceof FirebaseError && (
          <div className="mt-6 rounded-lg bg-red-500 p-2 text-center text-red-50 ">
            {formatFireabseAuthError(error)}
          </div>
        )}
        <label className="mt-6">E-mail</label>
        <input className="input mt-2" ref={email} />
        <label className="mt-6">Password</label>
        <input type={"password"} className="input mt-2" ref={password} />
        <div className="mt-8 flex items-center justify-between">
          <button type="submit" className="button ">
            Log in
          </button>
          <div className="text-xs ">
            Don't have an account?{" "}
            <Link className="text-slate-300 underline" href={"/auth/register"}>
              Sign Up
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

export default Login;
