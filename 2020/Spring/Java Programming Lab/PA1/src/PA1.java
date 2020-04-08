import java.util.ArrayList;
import java.util.List;

// Template code of "PA1.java"
// Do not change the format of the class and method in "PA1.java"
// Do not use java.util.LinkedList package

class Node<T> {
	T value = null;

	Node<T> prev = null;
	Node<T> next = null;

	Node(T val) {
		this.value = val;
	}
}

class Linkedlist<T> {
	Node<T> head = null;
	Node<T> tail = null;

	void addList(Linkedlist<T> list) {
		this.tail.next = list.head;
		list.head.prev = this.tail;
		this.tail = list.tail;
	}

	boolean equals(Linkedlist<T> list) {
		boolean result = true;
		if (this.size() != list.size())
			result = false;
		else {
			Node<T> tmp1 = this.head, tmp2 = list.head;
			while (tmp1 != null && tmp2 != null) {
				if (tmp1.value != tmp2.value) {
					result = false;
					break;
				} else {
					tmp1 = tmp1.next;
					tmp2 = tmp2.next;
				}
			}
		}
		return result;
	}

	int size() {
		if (head == tail) {
			if (head == null)
				return 0;
			else
				return 1;
		}
		Node<T> tmp = this.head;
		int len = 0;
		while (tmp != null) {
			len++;
			tmp = tmp.next;
		}
		return len;
	}

	void push_back(T val) {
		Node<T> newNode = new Node<T>(val);
		if (this.size() == 0) {
			tail = newNode;
			head = tail;
		} else {
			tail.next = newNode;
			newNode.prev = tail;
			tail = tail.next;
		}
	}

	void push_front(T val) {
		Node<T> newNode = new Node<T>(val);
		if (this.size() == 0) {
			head = newNode;
			tail = head;
		} else {
			head.prev = newNode;
			newNode.next = head;
			head = head.prev;
		}
	}

	String print_list() {
		Node<T> tmp = this.head;
		String res = "";
		while (tmp != null) {
			System.out.print(tmp.value + " ");
			res = res + tmp.value.toString() + " ";
			tmp = tmp.next;
		}
		System.out.println();
		return res;
	}

	T get(int n) {
		T value = null;
		if (this.size() <= n || this.size() == 0)
			return value;
		Node<T> tmp = this.head;
		int cnt = 0;
		while (cnt < n) {
			cnt++;
			tmp = tmp.next;
		}
		value = tmp.value;
		return value;
	}

	void set(int n, T val) {
		if (this.size() <= n || this.size() == 0)
			return;
		Node<T> tmp = this.head;
		int cnt = 0;
		while (cnt < n) {
			cnt++;
			tmp = tmp.next;
		}
		tmp.value = val;
		return;
	}

	void add(int n, T val) {
		if (this.size() <= n)
			return;
		if (n == 0)
			push_front(val);
		else {
			Node<T> newNode = new Node<T>(val);
			Node<T> tmp = this.head;
			int cnt = 0;
			while (cnt < n) {
				cnt++;
				tmp = tmp.next;
			}
			newNode.next = tmp;
			newNode.prev = tmp.prev;
			tmp.prev = newNode;
			tmp = newNode.prev;
			tmp.next = newNode;
		}
	}

	void erase(int n) {
		if (this.size() <= n)
			return;
		if (n == 0)
			pop_front();
		else {
			Node<T> tmp = this.head;
			int cnt = 0;
			while (cnt < n) {
				cnt++;
				tmp = tmp.next;
			}
			Node<T> nextNode = tmp.next;
			Node<T> prevNode = tmp.prev;
			prevNode.next = nextNode;
			nextNode.prev = prevNode;
		}
	}

	void pop_back() {
		if (this.size() == 0)
			return;
		// T tmp = this.tail.value;
		if (this.size() == 1) {
			this.head = null;
			this.tail = null;
		} else {
			tail = tail.prev;
			tail.next = null;
		}
		// return tmp;
	}

	void pop_front() {
		if (this.size() == 0)
			return;
		// T tmp = this.head.value;
		if (this.size() == 1) {
			this.head = null;
			this.tail = null;
		} else {
			head = head.next;
			head.prev = null;
		}

		// return tmp;
	}

	List<Linkedlist<T>> split(T val) {
		List<Linkedlist<T>> arr = new ArrayList<Linkedlist<T>>();
		Node<T> tmp = this.head;
		boolean flag = false;
		while (tmp != null) {
			if (tmp.value == val) {
				Linkedlist<T> list1 = new Linkedlist<T>(), list2 = new Linkedlist<T>();
				Node<T> tmp2 = tmp;
				while (tmp2 != null) {
					list1.push_front(tmp2.value);
					tmp2 = tmp2.prev;
				}
				tmp = tmp.next;
				while (tmp != null) {
					list2.push_back(tmp.value);
					tmp = tmp.next;
				}
				arr.add(list1);
				arr.add(list2);
				flag = true;
				break;
			}
		}
		if (!flag)
			arr.add(this);
		return arr;
	}

	Linkedlist() {
	}

	Linkedlist(T val) {
		head = new Node<T>(val);
		tail = head;
	}
}

public class PA1 {
	public static void main(String[] args) {
		// example of using Linkedlist with generic type
		Linkedlist<Integer> list = new Linkedlist<Integer>();
		Linkedlist<Integer> list2 = new Linkedlist<Integer>();
		Linkedlist<Integer> list3 = new Linkedlist<Integer>();
		for (int i = 1; i <= 10; i++)
			list.push_back(i);
		list.print_list();
		System.out.println(list.size());

		for (int i = 1; i <= 10; i++) {
			list2.push_front(i + 10);
			list3.push_front(i + 10);
		}
		list2.print_list();
		System.out.println(list2.equals(list3));
		System.out.println(list.equals(list2));

		list.pop_front();
		list.print_list();

		list.pop_back();
		list.print_list();

		list.add(1, 100);
		list.print_list();

		list.addList(list2);
		list.print_list();

		System.out.println(list.get(10));
		list.set(10, 125);
		System.out.println(list.get(10));
		list.add(10, 120);
		list.print_list();
		list.erase(11);
		list.print_list();
	}
}
