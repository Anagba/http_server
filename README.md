This project presents the design and implementation of a high-concurrency HTTP server developed entirely in the C programming language. The server utilizes the Berkeley Sockets API for TCP/IP communication and POSIX Threads (pthreads) for concurrent request processing.

The system adopts a thread-per-connection architecture, allowing multiple client requests to be handled simultaneously while maintaining isolation between client sessions. To enhance service availability and protect against excessive traffic bursts, an integrated Token Bucket rate-limiting mechanism is employed. The server also supports MIME type resolution for static web resources and delivers standards-compliant HTTP responses.

Key Features
Multi-threaded request handling using POSIX Threads
Berkeley Sockets-based TCP/IP communication
HTTP/1.1 request processing
Static file serving (HTML, CSS, JavaScript, PNG, JPEG, ICO)
MIME type detection and Content-Type generation
Integrated Token Bucket rate limiting
HTTP 429 (Too Many Requests) traffic control
Concurrent client servicing through a thread-per-connection model
Memory safety verified using Valgrind
Performance evaluation using ApacheBench
Technologies Used
C Programming Language
Berkeley Sockets API
POSIX Threads (pthreads)
GCC Compiler
ApacheBench (ab)
Valgrind
Ubuntu Linux
Performance Evaluation

The server was evaluated using ApacheBench under high-concurrency workloads. Experimental testing demonstrated successful concurrent request processing and effective enforcement of Token Bucket traffic regulation policies. Memory analysis performed using Valgrind confirmed the absence of definite memory leaks.
