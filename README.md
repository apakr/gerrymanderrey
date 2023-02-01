# Gerrymanderrey

This project is based on the mathematical definition of gerrymandering that I took from the following WIRED article: https://www.wired.com/story/the-math-behind-gerrymandering-and-wasted-votes/.

Gerrymandering is a controversial topic that involves creating voting districts to give one political party an advantage. However, it has always been difficult to quanitatively pinpoint exactly what determines a gerrymandered state and what doesn't. With the information that I got from reading the WIRED article I decided to put together a project that takes information from two files about different states, their eligible voters, the number of districts, the number of republican/democrat voters, etc. 
<br>Using all of this information the program goes through and calculates/stores the information to be later accessed by the user to find out the status of different states in terms of gerrymandering, and especially the percentages of how gerrymandered a state may be if it is deemed so.
<br>Some of the most important nuances that I gained from doing this project are:
* How programs in C++ can interact with other files
* A more thorough understanding of the actual code that goes into computational functions (like those in data libraries like pandas & Jupityr) looks like
* Rudimentary data analysis in the form of visualizations (through the console)
* Use of data structures in C++ (primarily vectors)

<br>Here are some examples of how the code looks when it is run: 

![image](https://user-images.githubusercontent.com/83194877/216173960-63e59a4c-00bb-4949-9631-2cdd74164774.png)

![image](https://user-images.githubusercontent.com/83194877/216174267-17522e22-caf3-4b4b-9a5c-4d7c9711a79a.png)

![image](https://user-images.githubusercontent.com/83194877/216174816-9099443c-f527-44fa-8557-bc3db281f838.png)

Overall, this project was quite enjoyable as it gave me a far more thorough understanding of what the term gerrymandering really means, as in contemporary times it is often thrown around to describe many political situations without much context of what it actually looks like. My favorite part was using file streams to read from files as doing something like that has always been a computing abstraction in my mind that I had no idea how to implement. However, after setting down and deciding to learn it for this project, then applying it successfully is a feeling that I truly cherish. 
For the future I hope that the skills that I learn from this project can be applied to better understand how the computing side of data analysis actually works, rather than it just being a function that you plug values and data containers into and then magically get a result back.
