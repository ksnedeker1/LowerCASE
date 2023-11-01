from PyQt5.QtGui import QSyntaxHighlighter, QTextCharFormat, QColor


class SyntaxHighlighter(QSyntaxHighlighter):
    def __init__(self, document):
        super(SyntaxHighlighter, self).__init__(document)
        # Instead of regex rules, store line numbers and their associated formats
        self.line_formats = {}

    def highlightBlock(self, text):
        """Overrides QSyntaxHighlighter.highlightBlock()"""
        # Check if current block's line number has a format to apply
        line_num = self.currentBlock().blockNumber()
        if line_num in self.line_formats:
            format = self.line_formats[line_num]
            self.setFormat(0, len(text), format)

    def highlight_line(self, line, color):
        """Stores a line number to be updated with the given color"""
        line -= 1
        format = QTextCharFormat()
        format.setBackground(QColor(color))
        # Store the line number with its format
        self.line_formats[line] = format
        self.rehighlightBlock(self.document().findBlockByNumber(line))

    def clear_highlighting(self):
        self.line_formats.clear()
        self.rehighlight()
