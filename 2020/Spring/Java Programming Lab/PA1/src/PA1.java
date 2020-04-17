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
		Linkedlist<T> temp = new Linkedlist<T>();
		for(int i = 0; i < list.size(); i++) {
			temp.push_back(list.get(i));
		}
		this.tail.next = temp.head;
		temp.head.prev = this.tail;
		this.tail = temp.tail;
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
		int size = this.size();
		if (size <= n || size == 0 || n < 0)
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
		int size = this.size();
		if (size <= n || size == 0 || n < 0)
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
		int size = this.size();
		if (size <= n || n < 0)
			return;
		if (n == 0)
			push_front(val);
		else if (n == size - 1)
			push_back(val);
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
		int size = this.size();
		if (size <= n || n < 0)
			return;
		if (n == 0)
			pop_front();
		else if(n == size - 1)
			pop_back();
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
		int size = this.size();
		if (size == 0)
			return;
		else if (size == 1) {
			this.head = null;
			this.tail = null;
		} else {
			tail = tail.prev;
			tail.next = null;
		}
		return;
	}

	void pop_front() {
		int size = this.size();
		if (size == 0)
			return;
		else if (size == 1) {
			this.head = null;
			this.tail = null;
		} else {
			head = head.next;
			head.prev = null;
		}
		return;
	}

	List<Linkedlist<T>> split(T val) {
		List<Linkedlist<T>> arr = new ArrayList<Linkedlist<T>>();
		List<Integer> index = new ArrayList<Integer>();
		int size = this.size();
		if(size == 0)
			return arr;
		Node<T> tmp = this.head;
		for(int i = 0; tmp != null; i++, tmp=tmp.next) {
			if(tmp.value == val)
				index.add(i);
		}
		
		if(index.size() == 0)
			arr.add(this);
		else {
			tmp = this.head;
			Linkedlist<T> temp = new Linkedlist<T>();
			for(int i = 0; tmp != null; i++, tmp = tmp.next) {
				if(index.contains(i)) {
					if(temp.size() != 0) {
						arr.add(temp);
						temp = new Linkedlist<T>();
					}
				}
				else {
					temp.push_back(tmp.value);
				}
			}
			if(temp.size() != 0) {
				arr.add(temp);
			}
		}
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

	}
}
