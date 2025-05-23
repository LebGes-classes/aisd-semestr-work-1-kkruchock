//ниже представленно кодирование текста с помощью кода Хаффмана, которое используется для сжатия данных
//для наглядности кодирование возвращает мапу кодов букв
//мапа имеет смысл только если по одному дереву Хаффмана нужно кодировать разные тексты в разное время, тогда доступ O(1) в мапе существенно ускорит
//в реальности, если кодировать один заданный текст, тратить память на мапу не имеет смысла
//поэтому кодировать можно сраз. Без мапы, реализуется немного проще, у меня усложеннная версия
//декодирование представленно в двух вариантах, как через мапу, так и через обход дерева

#include <string>
#include <unordered_map>
#include <iostream>
#include <queue>

using namespace std;

//узел дерева
struct Node {

	char ch;
	int weight;
	Node* leftHeir;
	Node* rightHeir;

	Node(char p_ch, int p_weight, Node* p_leftHeir, Node* p_rightHeir) {
		
		ch = p_ch;
		weight = p_weight;
		leftHeir = p_leftHeir;
		rightHeir = p_rightHeir;
	}
};

//подсчет кол-ва каждого символа в тексте
unordered_map<char, int> getCharWeightMap(string text) {

	unordered_map<char, int> charWeightMap;

	for (char ch : text) {
		charWeightMap[ch]++;
	}

	return charWeightMap;
}

//для приорететной кучи
struct CompareNodes {
	bool operator()(Node* x, Node* y) {

		return x->weight > y->weight;
	}
};


//в куче остается только корневая вершина
Node* makeHuffmanTree(string text) {

	priority_queue<Node*, vector<Node*>, CompareNodes> weightPriorityQueue;

	for (auto pair : getCharWeightMap(text)) {
		weightPriorityQueue.push(new Node(pair.first, pair.second, nullptr, nullptr));
	}

	while (weightPriorityQueue.size() > 1) {

		Node* x = weightPriorityQueue.top();
		weightPriorityQueue.pop();

		Node* y = weightPriorityQueue.top();
		weightPriorityQueue.pop();

		Node* parent = new Node('#', x->weight + y->weight, x, y);
		weightPriorityQueue.push(parent);
	}

	return weightPriorityQueue.top();
}

//основная логика кодирования, рекусрия
void encodeRecursion(Node* node, const string& currentCode, unordered_map<char, string>& huffmanCode) {

	//если дошли до листа
	if (node->leftHeir == nullptr && node->rightHeir == nullptr) {
		huffmanCode[node->ch] = currentCode;
		return;
	}

	//продолжаем обход, накапливая код
	encodeRecursion(node->leftHeir, currentCode + "0", huffmanCode);
	encodeRecursion(node->rightHeir, currentCode + "1", huffmanCode);
}

//функция кодирования вовращающая мапу кодов и символов
unordered_map<char, string> encodeChars(Node* root) {

	unordered_map<char, string> huffmanCode;
	encodeRecursion(root, "", huffmanCode);

	return huffmanCode;
}

//кодирование текста
string encodeText(const string& text, unordered_map<char, string> huffmanCode) {

	string encodedString;

	for (char ch : text) {

		encodedString += huffmanCode.at(ch);
	}
	return encodedString;
}

//декодирование через обход по дереву
string decodeWithTree(Node* root, string huffmanCode) {

	string resultText;
	Node* current = root;

	for (char bit : huffmanCode) {
		if (bit == '0') {
			current = current->leftHeir;  
		}
		else {
			current = current->rightHeir;
		}
		if (current->leftHeir == nullptr && current->rightHeir == nullptr) {
			resultText += current->ch;
			current = root;
		}
	}
	return resultText;

}

//декодирование через мапу
string decodeWithMap(unordered_map<char, string> codeMap, string huffmanCode) {

	unordered_map<string, char> decodeMap;
	for (const auto& pair : codeMap) {
		decodeMap[pair.second] = pair.first;  // Конвертируем мапу для быстрого доступа
	}

	string resultText;
	string currentCode;

	for (char bit : huffmanCode) {
		currentCode += bit;
		if (decodeMap.count(currentCode)) {
			resultText += decodeMap.at(currentCode);
			currentCode = "";
		}
	}
	return resultText;
}

int main() {

   string text1 = "Hello world!";
   
   cout << text1 << " - text" << endl;

   Node* root1 = makeHuffmanTree(text1);

   unordered_map<char, string> charCodeMap1 = encodeChars(root1);

   for (auto iter = charCodeMap1.begin(); iter != charCodeMap1.end(); ++iter) {
	   cout << iter->first << " => " << iter->second << endl;
   }

   string code1 = encodeText(text1, charCodeMap1);
   cout << code1 << " - text`s code" << endl;

   cout << decodeWithTree(root1, code1) << " - decoding with tree" << endl;
   cout << decodeWithMap(charCodeMap1, code1) << " - decoding with map" << endl;

   string text2 = "Laborum deserunt velit laboris amet cillum voluptate eiusmod exercitation officia. Sunt fugiat dolore enim excepteur laborum ipsum voluptate dolore reprehenderit aliqua anim adipisicing. Mollit enim minim labore anim veniam est consequat exercitation nostrud commodo. Ea aute fugiat laboris non esse nisi ea. Aute non ut labore eu enim ullamco ipsum est aliqua commodo elit magna amet id. Eu aute minim aute excepteur ut ea labore irure quis ex. Dolore excepteur eu eu cillum esse ad.";

   cout << "Now for longer text: " << endl;

   cout << text2 << " - text" << endl;

   Node* root2 = makeHuffmanTree(text2);

   unordered_map<char, string> charCodeMap2 = encodeChars(root2);

   for (auto iter = charCodeMap2.begin(); iter != charCodeMap2.end(); ++iter) {
	   cout << iter->first << " => " << iter->second << endl;
   }

   string code2 = encodeText(text2, charCodeMap2);
   cout << code2 << " - text`s code" << endl;

   cout << decodeWithTree(root2, code2) << " - decoding with tree" << endl;
   cout << decodeWithMap(charCodeMap2, code2) << " - decoding with map" << endl;

   return 0;
}