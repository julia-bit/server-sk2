import { initializeApp } from "firebase/app";
import { getAuth } from "firebase/auth";

const firebaseConfig = {
  apiKey: "AIzaSyBnPwyO1Bm0QzvtgGotZM5E9XLfw9sMBaQ",
  authDomain: "chat-room-f93da.firebaseapp.com",
  projectId: "chat-room-f93da",
  storageBucket: "chat-room-f93da.appspot.com",
  messagingSenderId: "108380571159",
  appId: "1:108380571159:web:a3b2af6a361bde97bbbe52",
};

const app = initializeApp(firebaseConfig);

export const auth = getAuth(app);
