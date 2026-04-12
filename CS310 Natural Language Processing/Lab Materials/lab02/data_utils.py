import torch
import re
from collections import defaultdict, Counter

###
# From https://pytorch.org/text/0.10.0/_modules/torchtext/data/functional.html
###
def to_map_style_dataset(iter_data):
    r"""Convert iterable-style dataset to map-style dataset.

    args:
        iter_data: An iterator type object. Examples include Iterable datasets, string list, text io, generators etc.


    Examples:
        >>> from torchtext.datasets import IMDB
        >>> from torchtext.data import to_map_style_dataset
        >>> train_iter = IMDB(split='train')
        >>> train_dataset = to_map_style_dataset(train_iter)
        >>> file_name = '.data/EnWik9/enwik9'
        >>> data_iter = to_map_style_dataset(open(file_name,'r'))
    """

    # Inner class to convert iterable-style to map-style dataset
    class _MapStyleDataset(torch.utils.data.Dataset):

        def __init__(self, iter_data):
            # TODO Avoid list issue #1296
            self._data = list(iter_data)

        def __len__(self):
            return len(self._data)

        def __getitem__(self, idx):
            return self._data[idx]

    return _MapStyleDataset(iter_data)


###
# From: https://pytorch.org/text/stable/_modules/torchtext/data/utils.html#get_tokenizer
###
_patterns = [r"\'", r"\"", r"\.", r"<br \/>", r",", r"\(", r"\)", r"\!", r"\?", r"\;", r"\:", r"\s+"]

_replacements = [" '  ", "", " . ", " ", " , ", " ( ", " ) ", " ! ", " ? ", " ", " ", " "]

_patterns_dict = list((re.compile(p), r) for p, r in zip(_patterns, _replacements))


def _split_tokenizer(x):  # noqa: F821
    # type: (str) -> List[str]
    return x.split()

def _basic_english_normalize(line):
    r"""
    Basic normalization for a line of text.
    Normalization includes
    - lowercasing
    - complete some basic text normalization for English words as follows:
        add spaces before and after '\''
        remove '\"',
        add spaces before and after '.'
        replace '<br \/>'with single space
        add spaces before and after ','
        add spaces before and after '('
        add spaces before and after ')'
        add spaces before and after '!'
        add spaces before and after '?'
        replace ';' with single space
        replace ':' with single space
        replace multiple spaces with single space

    Returns a list of tokens after splitting on whitespace.
    """

    line = line.lower()
    for pattern_re, replaced_str in _patterns_dict:
        line = pattern_re.sub(replaced_str, line)
    return line.split()


def get_tokenizer(tokenizer, language="en"):
    r"""
    Generate tokenizer function for a string sentence.

    Args:
        tokenizer: the name of tokenizer function. If None, it returns split()
            function, which splits the string sentence by space.
            If basic_english, it returns _basic_english_normalize() function,
            which normalize the string first and split by space. If a callable
            function, it will return the function. If a tokenizer library
            (e.g. spacy, moses, toktok, revtok, subword), it returns the
            corresponding library.
        language: Default en

    Examples:
        >>> import torchtext
        >>> from torchtext.data import get_tokenizer
        >>> tokenizer = get_tokenizer("basic_english")
        >>> tokens = tokenizer("You can now install TorchText using pip!")
        >>> tokens
        >>> ['you', 'can', 'now', 'install', 'torchtext', 'using', 'pip', '!']

    """

    # default tokenizer is string.split(), added as a module function for serialization
    if tokenizer is None:
        return _split_tokenizer

    if tokenizer == "basic_english":
        if language != "en":
            raise ValueError("Basic normalization is only available for Enlish(en)")
        return _basic_english_normalize


###
# Alternatives for iterator, vocab, and the helper function
###
class DatasetIterator:
    def __init__(self, dataset):
        self.dataset = dataset
        self.index = 0

    def __iter__(self):
        return self

    def __next__(self):
        if self.index < len(self.dataset):
            item = self.dataset[self.index]
            self.index += 1
            return item['sentence'], item['label']
        else:
            raise StopIteration

    def __len__(self):
        return len(self.dataset)
    
    def __getitem__(self, index):
        # Handle slicing or integer indexing
        if isinstance(index, slice):
            # Return a list of tuples for slicing
            return [(item['sentence'], item['label']) for item in self.dataset[index]]
        elif isinstance(index, int):
            # Return a single tuple for integer indexing
            item = self.dataset[index]
            return item['sentence'], item['label']
        else:
            raise TypeError("Invalid index type. Use an integer or slice.")

class Vocab:
    def __init__(self, word_to_idx):
        """
        Initialize the Vocab object.
        
        Args:
            word_to_idx (dict): A dictionary mapping words to their IDs.
        """
        self.word_to_idx = word_to_idx
        self.idx_to_word = {idx: word for word, idx in word_to_idx.items()}

    def __call__(self, words):
        """
        Convert a list of words to their corresponding IDs.
        Unknown words are mapped to ID 0.

        Args:
            words (list): A list of words to convert to IDs.

        Returns:
            list: A list of word IDs.
        """
        return [self.word_to_idx.get(word, 0) for word in words]

    def __len__(self):
        """
        Return the size of the vocabulary.
        """
        return len(self.word_to_idx)

    def __getitem__(self, word):
        """
        Get the ID for a given word.
        Unknown words return ID 0.
        """
        return self.word_to_idx.get(word, 0)

    def __contains__(self, word):
        """
        Check if a word is in the vocabulary.
        """
        return word in self.word_to_idx


def build_vocab_from_iter(iterator, specials=['<unk>'], min_freq=1):
    """
    Build a vocabulary from an iterator.

    Args:
        train_iter: An iterator yielding tuples of (sentence, label).
        specials (list): A list of special tokens to include at the beginning of the vocabulary.
        min_freq (int): Minimum frequency for a word to be included in the vocabulary.

    Returns:
        Vocab: A Vocab object.
    """
    # Count word frequencies
    word_freq = Counter()
    for tokens in iterator:
        word_freq.update(tokens)

    # Build word-to-idx mapping
    word_to_idx = {}
    # Assign IDs to special tokens first
    for idx, token in enumerate(specials):
        word_to_idx[token] = idx

    # Assign IDs to remaining words (non-special tokens)
    for word, freq in sorted(word_freq.items(), key=lambda x: x[1], reverse=True):
        if freq >= min_freq and word not in word_to_idx:  # Skip special tokens and low-frequency words
            word_to_idx[word] = len(word_to_idx)

    return Vocab(word_to_idx)