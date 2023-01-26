import cookies from "next-cookies";
import { GetServerSidePropsContext } from "next";
import { COOKIE_NAME } from "@/context/UserContext";

export const loggedInRedirect = (context: GetServerSidePropsContext) => {
  const cookie = cookies(context);

  if (cookie[COOKIE_NAME]) {
    console.log(cookie);
    return {
      redirect: {
        destination: "/",
        permanent: false,
      },
    };
  }
  return {
    props: {},
  };
};
