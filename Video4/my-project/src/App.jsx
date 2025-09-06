import { useState } from "react";

const API_BASE = "http://localhost:8848/api/v1/User"; // Change port if needed

function App() {
  const [userId, setUserId] = useState("");
  const [userName, setUserName] = useState("");
  const [userInfo, setUserInfo] = useState(null);
  const [message, setMessage] = useState("");

  const getUser = async () => {
    try {
      const res = await fetch(`${API_BASE}/${userId}`);
      const data = await res.json();
      setUserInfo(data);
    } catch (err) {
      console.error(err);
      setMessage("Error fetching user.");
    }
  };

  const createUser = async () => {
    try {
      const res = await fetch(`${API_BASE}/${userName}`, { method: "POST" });
      const data = await res.json();
      setMessage(`Created user: ${JSON.stringify(data)}`);
    } catch (err) {
      console.error(err);
      setMessage("Error creating user.");
    }
  };

  const updateUser = async () => {
    try {
      const res = await fetch(`${API_BASE}/${userId}`, {
        method: "PUT",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ name: userName }),
      });
      const data = await res.json();
      setMessage(`Updated user: ${JSON.stringify(data)}`);
    } catch (err) {
      console.error(err);
      setMessage("Error updating user.");
    }
  };

  const deleteUser = async () => {
    try {
      const res = await fetch(`${API_BASE}/${userId}`, { method: "DELETE" });
      const data = await res.json();
      setMessage(`Deleted user: ${JSON.stringify(data)}`);
    } catch (err) {
      console.error(err);
      setMessage("Error deleting user.");
    }
  };

  return (
    <div className="p-10 max-w-xl mx-auto">
      <h1 className="text-2xl font-bold mb-5">Drogon API Tester</h1>

      <div className="mb-4">
        <input
          type="text"
          placeholder="User ID"
          value={userId}
          onChange={(e) => setUserId(e.target.value)}
          className="border p-2 mr-2 rounded"
        />
        <input
          type="text"
          placeholder="User Name"
          value={userName}
          onChange={(e) => setUserName(e.target.value)}
          className="border p-2 mr-2 rounded"
        />
      </div>

      <div className="space-x-2 mb-4">
        <button onClick={getUser} className="bg-blue-500 text-white px-4 py-2 rounded">
          Get User
        </button>
        <button onClick={createUser} className="bg-green-500 text-white px-4 py-2 rounded">
          Create User
        </button>
        <button onClick={updateUser} className="bg-yellow-500 text-white px-4 py-2 rounded">
          Update User
        </button>
        <button onClick={deleteUser} className="bg-red-500 text-white px-4 py-2 rounded">
          Delete User
        </button>
      </div>

      {userInfo && (
        <div className="mb-4 p-4 border rounded bg-gray-50">
          <pre>{JSON.stringify(userInfo, null, 2)}</pre>
        </div>
      )}

      {message && <div className="text-green-600 font-semibold">{message}</div>}
    </div>
  );
}

export default App;
