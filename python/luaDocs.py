import re;
import collections;

warnings = [];
classStack = [];

def arrayJoin(arr, delim = " "):
	result = "";
	first = True;
	for line in arr:
		if len(line) == 0:
			continue;
		if not first:
			result += delim;
		first = False;
		result += line;
	return result;
	
def warning(message):
	warnings.append(arrayJoin(classStack, ".") + message);

class FunctionParam:
	def __init__(self, type, name):
		self.type = type;
		self.name = name;
		self.comment = "";
		self.commentJoin = [];
		self.optional = None;
		
	def finished(self):
		if len(self.commentJoin) == 0:
			warning(str(self) + " has no comment");
		self.comment = arrayJoin(self.commentJoin);
		self.commentJoin = None;
		
	def __str__(self):
		s = "(" + self.type;
		if self.name:
			s += " " + self.name;
		if self.optional:
			s += " [" + self.optional + "]";
		s += ")";
		if self.comment:
			s += " : " + self.comment;
		return s;

# Represents a single function signature for a potentially overloaded function.
# Contains a comment block, list of input paramters and returns.
# As well as if the signature is for a static function and a list of 'see also's.
class FunctionInstance:
	commentLineSplit = re.compile('/\*\*+(.*)|\s*\*?/?(.*)');
	metaMatch = re.compile('@\S+');
	whitespaceSplit = re.compile('\s+');
	def __init__(self):
		self.comment = "";
		self.params = [];
		self.returns = [];
		self.isStatic = False;
		self.parsing = None;
		self.alsoSee = [];
		self.inCode = False;
	def addParam(self, param):
		self.params.append(param)
		
	def idList(self):
		s = "";
		first = True;
		
		if len(self.params) == 0:
			s = "Void";
		else:
			for param in self.params:
				if first:
					first = False;
				else:
					s += "_";
				s += param.type + "_" + param.name;
		s += "__";
		if len(self.returns) == 0:
			s += "Void";
		else:
			first = True;
			for ret in self.returns:
				if first:
					first = False;
				else:
					s += "_";
				s += ret.type;
				
		return s;
	# Parses a single comment line.
	# Deals with looking for metatags such as @static, @param, @returns and @see.
	# Also replaces <br>'s with newlines.
	def parseLine(self, line):
		matchIter = FunctionInstance.metaMatch.finditer(line);
		for match in matchIter:
			meta = str(match.group()).lower();
			
			line = line.strip();
			
			if meta == "@static":
				self.isStatic = True;
				return "";
				
			if meta == "@param":
				# A param line can be in either format of
				# @param Type Name [Optional] Comment (Splits into 4)
				# or
				# @param Type Name Comment (Splits into 3)
				# So we need to split it into 4 and then combine 3 and 4
				# to make the whole comment if there is no optional.
				split = FunctionInstance.whitespaceSplit.split(line, 4);
				optional = None;
				name = split[2];
				type = split[1];
				comment = "";
				# Check for optional
				if name[0] == "[" and type[len(name) - 1] == "]":
					# Grab the optional from the square brackets.
					optional = name[1:len(name) - 1];
					name = split[3];
					if len(split) > 4:
						comment = split[4];
				else:
					# There is no optional so combine 3 and 4 if they are available.
					if len(split) > 3:
						comment = split[3];
						if len(split) > 3:
							comment += " " + split[4];
					
				param = FunctionParam(type, name);
				self.params.append(param);
				param.optional = optional;
				self.parsing = param;
				if len(split) > 3:
					return self.parseLine(comment);
				else:
					return "";
					
			if meta == "@returns":
				# Return tag has the format
				# @return Type Comment
				# So splitting is fairly simple.
				split = FunctionInstance.whitespaceSplit.split(line, 2);
				returnParam = FunctionParam(split[1], None);
				self.returns.append(returnParam);
				self.parsing = returnParam;
				if len(split) > 2:
					return self.parseLine(split[2]);
				else:
					return "";
					
			if meta == "@see":
				# See tag has the format
				# @see Type
				split = FunctionInstance.whitespaceSplit.split(line, 2);
				self.alsoSee.append(split[1]);
				# We don't need to include this line in the comments
				return "";
		
		code = line.find("<pre>");
		if code >= 0:
			self.inCode = True;
		code = line.find("</pre>");
		if code >= 0:
			self.inCode = False;
			
		if self.inCode:
			line = line + "\n";
			
		return line.replace("<br>", "\n");
		
	# Takes an array of unparsed comment lines and parses them into
	# the main comment block and the function parameters and returns.
	def parseComment(self, lines):
		commentJoin = [];
		for line in lines:
			for match in FunctionInstance.commentLineSplit.finditer(line):
				group = match.group(1) if match.group(1) else match.group(2);
				if group:
					s = self.parseLine(str(group));
					if len(s) > 0:
						if not self.parsing:
							commentJoin.append(s);
						else:
							self.parsing.commentJoin.append(s);
		
		self.comment = arrayJoin(commentJoin);
		
		for param in self.params:
			param.finished();
			
		for returnParam in self.returns:
			returnParam.finished();
			
		if len(self.comment) == 0:
			warning("." + str(self) + " has no comment");
		
		self.parsing = None;
	
	def __str__(self):
		s = "(";
		first = True;
		for param in self.params:
			if not first:
				s += ", ";
			first = False;
			s += param.type + " " + param.name;
			if param.optional:
				s += " [" + param.optional + "]";
		s += ") -> (";
		first = True;
		for returnParam in self.returns:
			if not first:
				s += ", ";
			first = False;
			s += returnParam.type;
		s += ")";
		return s;

# Represents a function with it's Lua name, C++ Name and all the overloaded
# function signatures it can have.
# Also stores if it's a special function like a constructor, destructor or operator.
class FunctionDescription:
	specialFuncNames = dict({"new":1, "__gc":1, "__eq":1});
	def __init__(self, name, cppName):
		self.name = name;
		self.cppName = cppName;
		self.instances = [];
		if name in FunctionDescription.specialFuncNames:
			self.isSpecial = True;
		else:
			self.isSpecial = False;
			
	def addInstance(self, instance):
		self.instances.append(instance);
		
	def parseComment(self, comment):
		instance = FunctionInstance();
		lines = comment.splitlines(False);
		classStack.append(self.name);
		# Parse lines for anything that affects all of the function instances.
		instance.parseComment(lines);
		self.instances.append(instance);
		classStack.pop();
		
	def __str__(self):
		return self.name + " (" + self.cppName + ")"

# Stores a representation of a class.
# This class can have a comment block, a name, a namespace and a list
# of all the functions and operators that this class has.
class ClassDescription:
	wrapperRegex = re.compile('"([^"]+)"[^,]*,\s*([^}]+)}');
	commentSplitter = re.compile("(/\*\*.*?\*/)|(int \w+\(lua_State[^\)]+\))", re.S);
	charParse = re.compile("int (\w+)\(lua_State");
	
	def __init__(self, name):
		self.name = name;
		self.classComment = "";
		self.namespace = None;
		# Special functions refer to ctor, dtor and operators
		self.specialFuncs = collections.OrderedDict();
		self.funcs = collections.OrderedDict();
		self.cppLookups = collections.OrderedDict();
		self.fileContents = "";
		self.wrappers = "";
		self.inCode = False;
		
	def addFunction(self, funcDesc):
		self.cppLookups[funcDesc.cppName] = funcDesc;
		if funcDesc.isSpecial:
			self.specialFuncs[funcDesc.name] = funcDesc;
		else:
			self.funcs[funcDesc.name] = funcDesc;
		
	# Parses a C++ wrapper file.
	# Looks for the luaL_Reg structure which contains all the mappings between
	# Lua functions and the C++ function names.
	# Then splits the file up into comments and function signatures and parses
	# comments to functions.
	def parseFile(self, filename):
		classStack.append(self.name);
		file = open(filename);
		self.fileContents = file.read();
		luaRegIndex = re.search("luaL_Reg", self.fileContents).start();
		bracketStart = self.fileContents.find("{", luaRegIndex);
		bracketEnd = self.fileContents.find("};", luaRegIndex);
		self.wrappers = self.fileContents[bracketStart + 1:bracketEnd];
		
		self.parseWrappers(self.wrappers);
		
		self.parseComments();
		
		classStack.pop();
		
	# Parses the luaL_Reg structure for mappings between the Lua function names
	# and the C++ function names.
	def parseWrappers(self, content):
		for match in ClassDescription.wrapperRegex.finditer(content):
			func = FunctionDescription(match.group(1), match.group(2).strip())
			self.addFunction(func);
			
	def parseClassLine(self, line):
		matchIter = FunctionInstance.metaMatch.finditer(line);
		for match in matchIter:
			meta = str(match.group()).lower();
			if meta == "@class":
				return "";
				
		code = line.find("<pre>");
		if code >= 0:
			self.inCode = True;
		code = line.find("</pre>");
		if code >= 0:
			self.inCode = False;
			
		if self.inCode:
			line = line + "\n";
				
		return line.replace("<br>", "\n");
			
	def parseClassComment(self, comment):
		commentJoin = [];
		for line in comment.splitlines(False):
			for match in FunctionInstance.commentLineSplit.finditer(line):
				group = match.group(1) if match.group(1) else match.group(2);
				if group:
					commentJoin.append(self.parseClassLine(str(group)));
						
		self.classComment = arrayJoin(commentJoin);
	# Splits the file into comments and function signatures.
	def parseComments(self):
		comments = [];
		for match in ClassDescription.commentSplitter.finditer(self.fileContents):
			line = match.group();
			index = line.find("/*");
			if index == 0:
				if line.find("@class") > 0:
					self.parseClassComment(line);
				else:
					comments.append(line);
			else:
				cppName = ClassDescription.charParse.match(line).group(1);
				
				if not cppName in self.cppLookups:
					continue
					
				func = self.cppLookups[cppName];
				
				if len(comments) == 0:
					warning("." + func.name + " has no comments");
				
				for comment in comments:
					func.parseComment(comment);
				comments = [];
		
		if len(self.classComment) == 0:
			warning(" has not class comment");
		
	def __str__(self):
		s = "Special functions\n"
		for k,v in self.specialFuncs.items():
			s += str(v) + "\n"
		s += "Normal functions\n"
		for k,v in self.funcs.items():
			s += str(v) + "\n"
			
		return s
		
class Documentation:
	whitespaceSplit = re.compile('\s+');
	def __init__(self):
		self.classes = collections.OrderedDict();
		
	def parseClass(self, className, filepath):
		classDesc = ClassDescription(className);
		classDesc.parseFile(filepath);
		self.classes[className] = classDesc;
		
	def parseList(self, listPath):
		basePath = "";
		file = open(listPath);
		contents = file.read();
		lines = contents.splitlines(False);
		for line in lines:
			#line = line.strip();
			if len(line) == 0 or line.find("//") == 0:
				continue;
				
			split = Documentation.whitespaceSplit.split(line, 2);
			if split[0] == "__basePath":
				basePath = split[1];
			else:
				classDesc = ClassDescription(split[0]);
				classDesc.parseFile(basePath + split[1]);
				self.classes[split[0]] = classDesc;
