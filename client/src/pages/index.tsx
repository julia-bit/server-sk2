import useChaters from "@/hooks/useChaters";
import Head from "next/head";
import Link from "next/link";
import { useEffect } from "react";
import { auth } from "@/lib/firebase";
import { signOut } from "firebase/auth";
import { useRouter } from "next/router";
import useUser from "@/hooks/useUser";
import { protectedRedirect } from "@/helpers/protectedRedirect";
import { NextPageContext } from "next";
import { GetServerSidePropsContext } from "next";
import useSocket from "@/hooks/useSocket";

function ChatRoom() {
  const { data: users, isLoading: loading } = useChaters();
  const { user, loading: loadingUser, error: userError } = useUser();
  const { socket } = useSocket();

  const router = useRouter();

  const logout = () => {
    if (socket) {
      socket.close();
    }
    signOut(auth);
    router.push("/auth/login");
  };
  if (loading || loadingUser) {
    return (
      <div className="flex h-screen w-screen flex-col justify-center bg-slate-800 text-center text-4xl font-bold text-slate-50">
        Loading ...
      </div>
    );
  }

  if (userError) {
    return (
      <div className="flex h-screen w-screen flex-col justify-center bg-slate-800 text-center text-4xl font-bold text-slate-50">
        Unexpected Error
      </div>
    );
  }

  return (
    <>
      <Head>
        <title>Welcome to chat-room</title>
      </Head>
      <div className="flex h-screen w-screen flex-col justify-center bg-slate-800">
        <h1 className="mx-auto text-xl text-slate-50">
          {auth.currentUser?.email}
        </h1>
        {user && (
          <button onClick={logout} className="button mx-auto my-6">
            Log out
          </button>
        )}
        <div className="mx-auto w-full max-w-xs divide-y rounded-lg border p-2 text-slate-50">
          {users?.map((user) => (
            <Link
              key={user?.user_id + user.username}
              href={"/chat-room/" + encodeURIComponent(user.user_id)}
            >
              <div
                className="rounded-md px-4 py-2 text-center transition hover:bg-slate-700"
                key={user.user_id}
              >
                {user.username}
              </div>
            </Link>
          ))}
        </div>
      </div>
    </>
  );
}

export function getServerSideProps(context: GetServerSidePropsContext) {
  const temp = protectedRedirect(context);

  return temp;
}

export default ChatRoom;
