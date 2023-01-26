import { User } from "firebase/auth";
import React, { useEffect, useState } from "react";
import { auth } from "../lib/firebase";
import Cookie from "js-cookie";

type UserContextType = {
  loading: boolean;
  error: Error | null;
  user: User | null;
};

export const UserContext = React.createContext<UserContextType>({
  loading: false,
  error: null,
  user: null,
});

export const COOKIE_NAME = "loggedin";

function UserProvider({ children }: { children: React.ReactNode }) {
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState<Error | null>(null);
  const [currentUser, setUser] = useState<User | null>(null);

  useEffect(() => {
    setLoading(true);
    const unsub = auth.onAuthStateChanged(
      (user) => {
        setError(null);
        setLoading(false);
        setUser(user);
        if (user) {
          Cookie.set(COOKIE_NAME, "yes");
        } else {
          Cookie.remove(COOKIE_NAME);
        }
      },
      (err) => {
        console.log(err);
        setError(err);
        setLoading(false);
        Cookie.remove(COOKIE_NAME);
      }
    );
    return () => {
      unsub();
    };
  }, []);

  return (
    <UserContext.Provider value={{ loading, user: currentUser, error }}>
      {children}
    </UserContext.Provider>
  );
}

export default UserProvider;
