(function (window) {
    const SPLIT_REGEX = /(\d+)\"(.*)\"(\d+)/;
    function Word(word, weight) {
        this.word = word || '';
        this.weight = weight || 0;
        this.next = null;
    }

    function NodeTree(c, weight, parent) {
        this.c = c || 0;
        this.weight = weight || 0;
        this.next = null;
        this.children = null;
        this.parent = parent || null;
    }

    class Predict {
        constructor() {
            this.total = 0;
            this.list = null;
            this.root = null;
        }

        sortNodeChildren(parent) {
            let a, b;
            a = parent.children;
            while (a) {
                b = a.next;
                if (b != null && b.weight > a.weight) {
                    a.next = b.next;
                    b.next = a;
                    if (parent.children == a) {
                        parent.children = b;
                    }
                    a = parent.children;
                } else {
                    a = a.next;;
                }
            }
        }

        printfChildren(children) {
            var x = '';
            while (children) {
                x += ` > ${children.c}`;
                children = children.next;
            }
            return x.slice(2);
        }

        pushNodeToTree(wordNode, parent, str, pos) {
            let children = parent.children;
            const c = str[pos] || 0;
            if (children == null) {
                const node = new NodeTree(c, wordNode.weight, parent);
                children = parent.children = node;
            }
            if (c === 0) {
                return;
            }
            while (children) {
                if (children.c === c) {
                    this.pushNodeToTree(wordNode, children, str, pos + 1);
                    if (children.weight < wordNode.weight) {
                        children.weight = wordNode.weight;
                        // this.sortNodeChildren(children);
                    }
                    return;
                } 
                if (children.next === null) {
                    const node = new NodeTree(c, wordNode.weight, parent);
                    children.next = node;
                    if (c !== 0) {
                        this.pushNodeToTree(wordNode, node, str, pos + 1);
                    }
                    return;
                }
                children = children.next;
            }
        }

        pushWordToTree(wordNode) {
            let node = this.root;
            let c = wordNode.word[0];
            if (node == null) {
                this.root = new NodeTree(c, wordNode.weight);
                node = this.root;
            }
            while (node) {
                if (node.c === c) {
                    if (node.weight < wordNode.weight) {
                        node.weight = wordNode.weight;
                    }
                    this.pushNodeToTree(wordNode, node, wordNode.word, 1);
                    return;
                } else if (node.next == null) {
                    const tmp = new NodeTree(c, wordNode.weight);
                    node.next = tmp;
                    this.pushNodeToTree(wordNode, tmp, wordNode.word, 1);
                    return;
                }
                node = node.next;
            }
        }

        addWordToDictionary(line) {
            const [
                fullLine,
                wordLen,
                word,
                weight
            ] = line.match(SPLIT_REGEX);
            if (!(word && word.length > 0)) {
                return;
            }
            // console.log(wordLen, word, weight);
            const wordNode = new Word(word, +weight);
            if (this.list == null) {
                this.list = wordNode;
            } else {
                wordNode.next = this.list;
                this.list = wordNode;
            }
        }

        loadFile(content) {
            const lines = (content || '').split('\n');
            let line;
            // Line = NUMBERS"WORD"WEIGHT
            const startTimer = window.performance.now();
            while (line = lines.shift()) {
                this.addWordToDictionary(line.toLowerCase());
            }
            this.generateTree();
            const endTimer = window.performance.now();
            console.log('Loaded in ' + (endTimer - startTimer) + 'ms');
        }

        generateTree() {
            const startTimer = window.performance.now();
            let wordNode = this.list;
            while (wordNode) {
                this.pushWordToTree(wordNode);
                wordNode = wordNode.next;
            }
            const endTimer = window.performance.now();
            console.log('Loaded in ' + (endTimer - startTimer) + 'ms');
        }

        searchInTree(treeNode, term) {
            let i = 1;
            let c = term[i] || 0;
            let n = treeNode.children;
            let pSearch = {
                'term': term,
                'diff': ''
            };
            while (n) {
                // console.log('c:', c, n);
                if (c !== 0 && n.c === c) { // Term found, go deeper
                    // console.log('found:', n);
                    c = term[++i] || 0;
                    n = n.children;
                } else if (c === 0) { // autopilote, go deeper
                    // console.log('found:', c, n);
                    if (n.c === 0) {
                        return pSearch;
                    }
                    pSearch.diff += n.c;
                    n = n.children;
                } else {
                    n = n.next;
                }
            }
            return null;
        }

        mPredict(term) {
            const startTimer = window.performance.now();
            const res =  this.predict(term);
            const endTimer = window.performance.now();
            console.log('Loaded in ' + (endTimer - startTimer) + 'ms');
            return res;
        }

        predict(term) {
            term = term.toLowerCase();
            let node = this.root;
            let c = term[0];
            while (node) {
                if (node.c == c) {
                    return this.searchInTree(node, term);
                }
                node = node.next;
            }
            return null;
        }
    }
    window.Predict = new Predict();
})(window);