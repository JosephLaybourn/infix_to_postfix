#include "infix2postfix.h"
#include <stack>
#include <iostream>

static bool isOperator(unsigned char check);
static bool isHigherPrecedence(unsigned char stackTop, unsigned char check);
static void popAndSend(std::string &input, std::stack<unsigned char> &output);

int Evaluate(std::string const & postfix)
{
  std::stack<char> stack; //( postfix.length() );

  for (std::string::const_iterator it = postfix.begin(); it != postfix.end(); ++it) {
    char token = *it;

    if (token >= '0' && token <= '9') {
      stack.push(token - '0');
    }
    else {
      int arg2 = stack.top();
      stack.pop();
      int arg1 = stack.top();
      stack.pop();

      switch (token)
      {
      case '+':
        stack.push(arg1 + arg2);
        break;

      case '-':
        stack.push(arg1 - arg2);
        break;

      case '*':
        stack.push(arg1 * arg2);
        break;

      case '/':
        stack.push(arg1 / arg2);
        break;

      default:
        std::cout << "Unknown operator" << std::endl;
        return 0;
      }
    }
  }
  return stack.top();
}

std::string Infix2postfix(std::string const & infix)
{
  std::string postfix;
  std::stack<unsigned char> cache;

  for (unsigned i = 0; i < infix.size(); i++)
  {
    unsigned char check = infix[i];

    // Operand - send to the output
    if (check >= '0' && check <= '9')
    {
      postfix += check;
      continue;
    }

    // Left parenthesis - push onto the stack
    if (check == '(')
    {
      cache.push(check);
      continue;
    }

    // Right parenthesis - operators are popped off the stack and sent to the output until a left parenthesis is found(and then discarded).
    if (check == ')')
    {
      popAndSend(postfix, cache);
      continue;
    }

    // Operator
    if(isOperator(check))
    {
      // If the stack is empty, push the operator.
      // If the top of the stack is a left parenthesis, push the operator onto the stack.
      // If the top of the stack is an operator which has the same or lower precendence than the scanned operator, push the scanned operator.
      if ((!(cache.size())) || cache.top() == '(' || !isHigherPrecedence(check, cache.top()))
      {
        cache.push(check);
        continue;
      }

      // If the top of the stack is an operator which has higher precedence, pop the stack and send to the output.Repeat the algorithm with the new top of stack.
      unsigned int cacheSize = cache.size();
      for(unsigned i = 0; i < cacheSize; i++)
      {
        if (isHigherPrecedence(check, cache.top()))
        {
          postfix += cache.top();
          cache.pop();
        }
      }
      cache.push(check);
    }
  }
  // If the input stream is empty and there are still operators on the stack, pop all of them and add them to the output.
  if (cache.size())
  {
    popAndSend(postfix, cache);
  }

  return postfix;
}

static bool isOperator(unsigned char check)
{
  if (check == '+' ||
      check == '-' ||
      check == '*' ||
      check == '/'   )
  {
    return true;
  }

  else
  {
    return false;
  }
}

static bool isHigherPrecedence(unsigned char check, unsigned char stackTop)
{
  if ((stackTop == '*' || stackTop == '/') &&
      (check == '+' || check == '-'))
  {
    return true;
  }
  else
  {
    return false;
  }
}

void popAndSend(std::string &input, std::stack<unsigned char> &output)
{
  unsigned stackSize = output.size();
  for (unsigned j = 0; j < stackSize; j++)
  {
    if (output.top() == '(')
    {
      output.pop();
      break;
    }
    input += output.top();
    output.pop();
  }
}
