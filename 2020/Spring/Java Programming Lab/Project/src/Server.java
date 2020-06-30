import java.io.*;
import java.net.*;
import java.util.*;

public class Server {
	private ArrayList<Thread> threads;
	private HashMap<String, DataOutputStream> clients;
	private ServerSocket ss;

	/* start server */
	public static void main(String[] args) {
		new Server();
	}

	/**
	 * ctor of Server
	 */
	public Server() {
		threads = new ArrayList<Thread>();
		clients = new HashMap<String, DataOutputStream>();
		Collections.synchronizedMap(clients);
		start();
	}

	public void start() {
		try {
			ss = new ServerSocket(5000);
		} catch (IOException e) {
			e.printStackTrace();
		}
		System.out.println("server is ready");

		while (true) {
			try {
				ArrayList<Thread> removethreads = new ArrayList<Thread>();
				Socket soc = ss.accept();
				System.out.println("new connection arrived");
				Thread t = new Receive(soc);
				t.start();

				threads.add(t);
				for (Thread th : threads) {
					if (!th.isAlive()) {
						removethreads.add(th);
					}
				}
				threads.removeAll(removethreads);
				removethreads.clear();
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
	}

	public synchronized void addClient(String name, Socket soc) throws IOException {
		clients.put(name, new DataOutputStream(soc.getOutputStream()));
		System.out.println("User '" + name + "' added");
		updateName();
	}

	public synchronized void removeClient(String name) throws IOException {
		clients.remove(name);
		System.out.println("User '" + name + "' removed");
		updateName();
	}

	public synchronized void updateName() throws IOException {
		System.out.println("Update user names");
		Iterator<String> it = clients.keySet().iterator();
		while (it.hasNext()) {
			String client_name = it.next();
			clients.get(client_name).writeInt(0);
			clients.get(client_name).writeInt(clients.keySet().size());
			Iterator<String> it2 = clients.keySet().iterator();
			while (it2.hasNext()) {
				String name = it2.next();
				clients.get(client_name).writeUTF(name);
			}
			clients.get(client_name).flush();
		}
	}

	public synchronized void sendFDSchedule(String to_send, String name, Boolean isImp, String memo, int RepeatType,
			boolean isFullDay, int[] start, int color) throws Exception {
		Iterator<String> it = clients.keySet().iterator();
		while (it.hasNext()) {
			String client_name = it.next();
			if (client_name.equals(to_send)) {
				clients.get(client_name).writeInt(1);
				clients.get(client_name).writeUTF(name);
				clients.get(client_name).writeBoolean(isImp);
				clients.get(client_name).writeUTF(memo);
				clients.get(client_name).writeInt(RepeatType);
				clients.get(client_name).writeInt(color);
				clients.get(client_name).writeBoolean(true);
				for (int i = 0; i < 3; i++) {
					clients.get(client_name).writeInt(start[i]);
				}
				clients.get(client_name).flush();
			}
		}
	}

	public synchronized void sendNormSchedule(String to_send, String name, Boolean isImp, String memo, int RepeatType,
			boolean isFullDay, int[] start, int[] end, boolean canBeOverlapped, int color) throws Exception {
		Iterator<String> it = clients.keySet().iterator();
		while (it.hasNext()) {
			String client_name = it.next();
			if (client_name.equals(to_send)) {
				clients.get(client_name).writeInt(1);
				clients.get(client_name).writeUTF(name);
				clients.get(client_name).writeBoolean(isImp);
				clients.get(client_name).writeUTF(memo);
				clients.get(client_name).writeInt(RepeatType);
				clients.get(client_name).writeInt(color);
				clients.get(client_name).writeBoolean(false);
				for (int i = 0; i < 5; i++) {
					clients.get(client_name).writeInt(start[i]);
					clients.get(client_name).writeInt(end[i]);
				}
				clients.get(client_name).writeBoolean(canBeOverlapped);
				clients.get(client_name).flush();
			}
		}
	}

	class Receive extends Thread {
		Socket soc;
		DataInputStream dis;
		DataOutputStream dos;
		String name;

		public Receive(Socket soc) throws Exception {
			this.soc = soc;
			dis = new DataInputStream(soc.getInputStream());
			dos = new DataOutputStream(soc.getOutputStream());
			while (true) {
				try {
					name = dis.readUTF();
					if (!clients.keySet().contains(name)) {
						dos.writeBoolean(true);
						break;
					} else {
						dos.writeBoolean(false);
					}
				} catch (IOException e) {
				}
				dos.flush();
			}
			addClient(name, soc);
		}

		public void run() {
			try {
				while (true) {
					String to_send = dis.readUTF();
					String name = dis.readUTF();
					Boolean isImp = dis.readBoolean();
					String memo = dis.readUTF();
					int RepeatType = dis.readInt();
					int colorRGB = dis.readInt();
					boolean isFullDay = dis.readBoolean();
					if (isFullDay) {
						int[] start = new int[3];
						for (int i = 0; i < 3; i++) {
							start[i] = dis.readInt();
						}
						sendFDSchedule(to_send, name, isImp, memo, RepeatType, isFullDay, start, colorRGB);
						System.out.println("Send '" + name + "' schedule to '" + to_send + "'");
					} else {
						int[] start = new int[5];
						int[] end = new int[5];
						for (int i = 0; i < 5; i++) {
							start[i] = dis.readInt();
							end[i] = dis.readInt();
						}
						boolean canBeOverlapped = dis.readBoolean();
						sendNormSchedule(to_send, name, isImp, memo, RepeatType, isFullDay, start, end, canBeOverlapped, colorRGB);
						System.out.println("Send '" + name + "' to '" + to_send + "'");
					}
				}
			} catch (Exception e) {
				try {
					removeClient(this.name);
				} catch (IOException e1) {}		
			}
		}
	}	
}
