import requests
from bs4 import BeautifulSoup
from urllib.parse import urljoin
from sentence_transformers import SentenceTransformer, util

# Load semantic similarity model
similarity_model = SentenceTransformer("all-MiniLM-L6-v2")
visited = set()

def fetch_page(url):
    """Fetch the content of a web page."""
    try:
        response = requests.get(url)
        response.raise_for_status()
        return response.text
    except requests.exceptions.RequestException as e:
        print(f"Error fetching {url}: {e}")
        return None

def extract_links(html, base_url):
    """Extract hyperlinks from a web page."""
    soup = BeautifulSoup(html, "html.parser")
    return [urljoin(base_url, link.get("href")) for link in soup.find_all("a", href=True)]

def rank_pages(pages, query):
    """Rank pages based on semantic relevance to a query."""
    query_embedding = similarity_model.encode(query, convert_to_tensor=True)
    for page in pages:
        page_embedding = similarity_model.encode(page["content"], convert_to_tensor=True)
        page["similarity"] = util.pytorch_cos_sim(query_embedding, page_embedding).item()

    return sorted(pages, key=lambda page: page["similarity"], reverse=True)

def main():
    query = "example search query"
    seed_url = "https://example.com"

    html = fetch_page(seed_url)
    if not html:
        print("Failed to fetch page")
        return

    links = extract_links(html, seed_url)
    links = [link for link in links if link not in visited]
    visited.update(links)

    pages = [{"url": link, "content": fetch_page(link)} for link in links]
    pages = [page for page in pages if page["content"]]
    ranked_pages = rank_pages(pages, query)

    for page in ranked_pages[:10]:
        print(page["url"])

if __name__ == "__main__":
    main()
