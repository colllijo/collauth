#include "http/HTTPStatus.hpp"

std::string statusToString(HTTPStatus status)
{
	switch (status)
	{
	case HTTPStatus::OK:
		return "200 OK";
	case HTTPStatus::BAD_REQUEST:
		return "400 Bad Request";
	case HTTPStatus::NOT_FOUND:
		return "404 Not Found";
	case HTTPStatus::INTERNAL_SERVER_ERROR:
		return "500 Internal Server Error";
	default:
		return "500 Internal Server Error";
	}
}
