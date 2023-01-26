import { COOKIE_NAME } from "@/context/UserContext";
import { GetServerSidePropsContext } from "next";
import cookies from "next-cookies";

export const protectedRedirect = (context: GetServerSidePropsContext) => {
  const cookie = cookies(context);

  if (!cookie[COOKIE_NAME]) {
    return {
      redirect: {
        destination: "/auth/login",
        permanent: false,
      },
    };
  }
  return {
    props: {},
  };
};
